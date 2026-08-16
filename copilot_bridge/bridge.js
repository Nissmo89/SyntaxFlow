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
        
        client = new CopilotClient({ token });
        await client.start();
        sendResponse(id, { status: "initialized" });
        break;

      case "create_session":
        if (!client) throw new Error("Client not initialized");
        const session = await client.createSession({ model: params.model || "gpt-4" }); // Defaults based on SDK
        const sessionId = Math.random().toString(36).substring(7);
        sessions.set(sessionId, session);
        sendResponse(id, { sessionId });
        break;

      case "get_completion":
        if (!client) throw new Error("Client not initialized");
        const { code, language, cursorOffset } = params;
        
        // This simulates a completion request for CodeMirror inline ghost text.
        // The actual SDK method might be `session.send()` or `session.complete()`.
        // We will structure it as a typical Copilot prompt.
        const activeSession = sessions.get(params.sessionId) || await client.createSession({ model: "gpt-4" });
        
        // Request completion
        const response = await activeSession.send({
          prompt: code,
          language: language,
          cursorOffset: cursorOffset
        });

        sendResponse(id, { completion: response });
        break;
        
      case "chat":
        if (!client) throw new Error("Client not initialized");
        const { prompt, codeContext, languageContext } = params;
        const chatSession = sessions.get(params.sessionId) || await client.createSession({ model: "gpt-4" });
        
        let fullPrompt = `The user is writing ${languageContext} code. Here is the current code context:\n\`\`\`${languageContext}\n${codeContext}\n\`\`\`\n\nUser query: ${prompt}`;
        
        const chatResponse = await chatSession.send({ prompt: fullPrompt });
        sendResponse(id, { 
            chat: chatResponse.text || chatResponse.completion || (typeof chatResponse === 'string' ? chatResponse : JSON.stringify(chatResponse)) 
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
