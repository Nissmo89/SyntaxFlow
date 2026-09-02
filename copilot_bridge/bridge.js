import { CopilotClient } from "@github/copilot-sdk";

import fs from 'fs';

fs.writeFileSync('bridge_debug.log', `[${new Date().toISOString()}] Bridge started with Node ${process.version}\n`);

// Map to hold active sessions
const sessions = new Map();
let client = null;

// Send JSON response to C++ backend
function sendResponse(id, result, error = null) {
  const msg = JSON.stringify({ id, result, error }) + "\n";
  fs.appendFileSync('bridge_debug.log', `[OUT] ${msg}`);
  process.stdout.write(msg);
}

process.on('uncaughtException', err => {
  fs.appendFileSync('bridge_debug.log', `[FATAL] ${err.stack}\n`);
  process.stderr.write(`FATAL: ${err.stack}\n`);
  process.exit(1);
});
process.on('unhandledRejection', err => {
  fs.appendFileSync('bridge_debug.log', `[REJECTION] ${err.stack}\n`);
  process.stderr.write(`REJECTION: ${err.stack}\n`);
});

async function handleRequest(request) {
  const { id, method, params } = request;

  try {
    switch (method) {
      case "init":
        const token = params.token;
        if (!token) throw new Error("GitHub token is required for initialization");
        
        client = new CopilotClient({ gitHubToken: token });
        await client.start();
        sendResponse(id, { status: "initialized" });
        break;

      case "create_session":
        if (!client) throw new Error("Client not initialized");
        const session = await client.createSession(params.model ? { model: params.model } : undefined);
        const sessionId = Math.random().toString(36).substring(7);
        sessions.set(sessionId, session);
        sendResponse(id, { sessionId });
        break;

      case "get_completion":
        if (!client) throw new Error("Client not initialized");
        const { code, language, cursorOffset } = params;
        
        // This simulates a completion request for CodeMirror inline ghost text.
        const activeSession = sessions.get(params.sessionId) || await client.createSession();
        
        // Request completion
        const compResponse = await activeSession.sendAndWait({
          prompt: code,
          language: language,
          cursorOffset: cursorOffset
        });

        sendResponse(id, { completion: compResponse?.data?.content || "" });
        break;
        
      case "chat":
        if (!client) throw new Error("Client not initialized");
        const { prompt, context } = params;
        const chatSession = sessions.get(params.sessionId) || await client.createSession();
        
        const systemPrompt = `You are SyntaxFlow Assistant, an AI programming assistant for competitive programming and algorithmic problem solving.

Your primary goals are:
1. Help the user understand programming problems.
2. Help users develop correct algorithms.
3. Debug and analyze submitted code.
4. Explain mistakes clearly.
5. Improve inefficient solutions.
6. Teach problem-solving techniques rather than merely producing answers.

GENERAL BEHAVIOR
- Understand the user's intent before responding.
- Prefer correctness over brevity.
- Do not invent compiler output, test results, or execution results.
- When tools are available, use them instead of guessing.
- Do not modify code unless the user asks for a fix, implementation, or rewrite.
- Preserve the user's programming language unless they explicitly request another language.

COMPETITIVE PROGRAMMING
When solving a problem:
1. Understand the problem.
2. Identify constraints.
3. Determine the required complexity.
4. Develop an algorithm.
5. Check edge cases.
6. Implement the solution.
7. Verify the implementation when execution tools are available.
8. Explain the final approach.

DEBUGGING
When debugging:
1. Reproduce the problem if possible.
2. Inspect compiler/runtime errors.
3. Identify the smallest failing case.
4. Locate the root cause.
5. Explain why the bug occurs.
6. Provide a corrected implementation if appropriate.

HINT MODE
When the user asks for a hint:
- Do not immediately reveal the complete solution.
- Give progressively stronger hints.
- Preserve the educational value of the problem.

CODE
- Always preserve indentation and formatting.
- Use fenced Markdown code blocks.
- Specify the language of code blocks.
- Do not minify code.
- Do not unnecessarily rewrite working code.

RESPONSE STYLE
Be like an experienced competitive programmer helping another programmer.
Prefer: short explanations, clear reasoning, concrete examples, complexity analysis, edge cases.
Avoid: unnecessary introductions, generic motivational text, repeating the user's question, unrelated explanations.`;

        let mode = "SOLVE";
        const lowerPrompt = prompt.toLowerCase();
        if (lowerPrompt.includes("hint") || lowerPrompt.includes("stuck")) mode = "HINT";
        else if (lowerPrompt.includes("tle") || lowerPrompt.includes("optimize") || lowerPrompt.includes("faster") || lowerPrompt.includes("complexity")) mode = "OPTIMIZE";
        else if (lowerPrompt.includes("fail") || lowerPrompt.includes("wrong answer") || lowerPrompt.includes("error") || lowerPrompt.includes("debug") || lowerPrompt.includes("why")) mode = "DEBUG";
        else if (lowerPrompt.includes("explain") || lowerPrompt.includes("how does")) mode = "EXPLAIN";
        
        let contextEng = `TASK TYPE: ${mode}\n`;
        contextEng += `LANGUAGE: ${context?.lang || 'unknown'}\n`;
        
        if (context?.problem && context.problem.trim() !== "Loading..." && context.problem.length > 10) {
            contextEng += `\nPROBLEM:\n${context.problem}\n`;
        }
        
        if (context?.lastExecution && !context.lastExecution.includes("Run your code to see submissions here")) {
            contextEng += `\nLAST COMPILATION / EXECUTION:\n${context.lastExecution}\n`;
        }
        
        if (context?.selection && context.selection.trim().length > 0) {
            contextEng += `\nSELECTED CODE:\n\`\`\`${context?.lang || 'cpp'}\n${context.selection}\n\`\`\`\n`;
        } else if (context?.code && context.code.trim().length > 0) {
            contextEng += `\nUSER CODE:\n\`\`\`${context?.lang || 'cpp'}\n${context.code}\n\`\`\`\n`;
        }
        
        contextEng += `\nUSER QUERY:\n${prompt}`;
        let fullPrompt = `${systemPrompt}\n\n====================\n\n${contextEng}`;
        
        const chatResponse = await chatSession.sendAndWait({ prompt: fullPrompt });
        sendResponse(id, { 
            chat: chatResponse?.data?.content || "No response generated."
        });
        break;
        
      case "stop":
        if (client) {
            await client.stop();
        }
        process.exit(0);
        break;

      default:
        throw new Error(`Unknown method: ${method}`);
    }
  } catch (err) {
    sendResponse(id, null, err.message || String(err));
  }
}

// Read from stdin line-by-line
let buffer = "";
process.stdin.on("data", (data) => {
  buffer += data.toString();
  let newlineIndex;
  while ((newlineIndex = buffer.indexOf("\n")) >= 0) {
    const line = buffer.slice(0, newlineIndex).trim();
    buffer = buffer.slice(newlineIndex + 1);
    
    if (line) {
      fs.appendFileSync('bridge_debug.log', `[IN] ${line}\n`);
      try {
        const req = JSON.parse(line);
        handleRequest(req);
      } catch (err) {
        process.stderr.write(`Failed to parse JSON: ${err.message}\n`);
      }
    }
  }
});
