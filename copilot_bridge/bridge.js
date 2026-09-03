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
        const isInternalAgentLoop = context?.isInternalAgentLoop === true;
        const chatSession = sessions.get(params.sessionId) || await client.createSession();
        
        const agentSystemPrompt = `You are SyntaxFlow Agent, an autonomous competitive programming assistant.
You do NOT just passively output code. You have tools to read, compile, test, and debug code.

AVAILABLE TOOLS:
1. {"tool": "get_problem"} - Returns the problem description and constraints.
2. {"tool": "get_code"} - Returns the current code in the editor.
3. {"tool": "compile_and_run", "args": {"code": "..."}} - Overwrites the editor with new code and runs all test cases.
4. {"tool": "run_test_cases"} - Runs test cases on the current editor code.
5. {"tool": "answer_user", "args": {"message": "..."}} - Sends a formatted Markdown message back to the user. Use this when you have finished your reasoning/tools and are ready to talk to the user.

RULES:
- When you want to use a tool, you MUST output ONLY the raw JSON object and absolutely nothing else.
- DO NOT wrap the JSON in markdown code blocks.
- You must reason about the user's request, gather information using tools, test your code, and THEN answer the user.
- If the user asks a simple question ("hi"), you can just use "answer_user" immediately.
- If the user asks you to solve or debug, use "get_problem" or "get_code", then write and test code with "compile_and_run", analyze the results, and finally "answer_user".
- IMPORTANT: The platform uses a hidden, automated test runner. When you write code, ONLY write the Solution class or required function. NEVER append manual test cases or `if __name__ == '__main__':` blocks to the code, as this pollutes the user's editor and conflicts with the automated sandbox.
- IMPORTANT: If your code causes an unexpected system crash or consistently fails with "null" output multiple times, ASSUME there is a backend sandbox/compiler issue. DO NOT loop infinitely trying to fix it. Instead, use "answer_user" to inform the user about the suspected system error.
`;

        let fullPrompt = prompt;
        
        // Only inject the heavy system prompt and context on the initial user message
        if (!isInternalAgentLoop) {
            let mode = "SOLVE";
            const lowerPrompt = prompt.toLowerCase();
            if (lowerPrompt.includes("hint") || lowerPrompt.includes("stuck")) mode = "HINT";
            else if (lowerPrompt.includes("tle") || lowerPrompt.includes("optimize")) mode = "OPTIMIZE";
            else if (lowerPrompt.includes("fail") || lowerPrompt.includes("debug") || lowerPrompt.includes("why")) mode = "DEBUG";
            
            let contextEng = `USER REQUEST: ${prompt}\n\n`;
            
            fullPrompt = `${agentSystemPrompt}\n\n====================\n\n${contextEng}\nPlease begin your agentic loop by choosing a tool.`;
        }
        
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
