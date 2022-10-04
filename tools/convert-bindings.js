// clang-format off
const fs = require("fs").promises;
const pathUtil = require("path");

// Base path should point to the main directory of the repo
if (process.argv.length < 3) {
    showLog("Missing 'basePath' argument, exiting...");
    process.exit(1);
}
const basePath = process.argv[2];

// Paths to search for JS bindings
const paths = [
    "shared/bindings/",
    "client/src/bindings/",
    "server/src/bindings/"
];

// Full output file
const resultTemplate = `// !!! THIS FILE WAS AUTOMATICALLY GENERATED, DO NOT EDIT MANUALLY !!!
#pragma once
#include <string>

namespace JSBindings {
    static std::string GetBindingsCode()
    {
        static constexpr char code[] = { {BindingsCode},'\\0' };
        return code;
    }
}
`;

// Result bindings output path
const outputPath = "shared/JSBindings.h";

(async() => {
    // todo: support client/server only bindings
    showLog("Generating bindings...");
    const bindings = [];
    for (const path of paths) {
        const bindingsPath = pathUtil.resolve(__dirname, basePath, path);
        for await(const file of getBindingFiles(bindingsPath)) {
            const fullFilePath = pathUtil.resolve(bindingsPath, file);
            // Generate the binding data
            const binding = await generateBinding(fullFilePath);
            bindings.push(binding);
            showLog(`Generated bindings for: ${file}`);
        }
    }
    const fullBindingsCode = bindings.flat();
    const outputStr = resultTemplate.replace("{BindingsCode}", fullBindingsCode.toString());
    await fs.writeFile(pathUtil.resolve(__dirname, basePath, outputPath), outputStr);
    showLog(`Wrote bindings result to file: ${outputPath}`);
})();

async function generateBinding(path) {
    const fileContent = await fs.readFile(path, "utf8");
    // Convert the whole file content to a char code array
    const chars = fileContent.split("").map((char) => char.charCodeAt(0));
    return chars;
}

// Recursively gets all binding files in the directory, returns an async iterator
async function* getBindingFiles(dir) {
    const items = await fs.readdir(dir, { withFileTypes: true });
    for (const item of items) {
        const path = pathUtil.resolve(dir, item.name);
        if(!path.endsWith(".js")) continue;
        if(item.isDirectory()) yield* getFiles(path);
        else yield path;
    }
}

function showLog(...args) {
    const date = new Date();
    const hours = date.getHours(), minutes = date.getMinutes(), seconds = date.getSeconds();
    const time = `[${hours < 10 ? `0${hours}` : hours}:${minutes < 10 ? `0${minutes}` : minutes}:${seconds < 10 ? `0${seconds}` : seconds}]`;
    console.log(time, ...args);
}
