// clang-format off
const fs = require("fs").promises;
const pathUtil = require("path");
const util = require("util");

// Base path should point to the main directory of the repo
if (process.argv.length < 3) process.exit(1);
const basePath = process.argv[2];

// Paths to search for JS bindings
const paths = [
    "shared/bindings/",
    "client/src/bindings/",
    "server/src/bindings/"
];

// Template for binding output text
const template = `#pragma once
namespace {Name}Bindings {
    static constexpr char source[] = { {Chars},'\\0' };
}
`;

(async() => {
    showLog("Generating bindings...");
    for (const path of paths) {
        const bindingsPath = pathUtil.resolve(__dirname, basePath, path);
        for await(const file of getBindingFiles(bindingsPath)) {
            const fullFilePath = pathUtil.resolve(bindingsPath, file);
            const fileContent = await fs.readFile(fullFilePath, "utf8");
            // Convert the whole file content to a char code array
            const chars = fileContent.split("").map((char) => char.charCodeAt(0));
            const bindingName = pathUtil.basename(fullFilePath, ".js");
            // Fill output template with our data
            const outputStr = template
                .replace("{Name}", bindingName)
                .replace("{Chars}", chars.toString());
            const fileName = `${bindingName}.out.h`;
            // Write output file
            await fs.writeFile(pathUtil.resolve(bindingsPath, fileName), outputStr);
            console.log(`Wrote bindings to file: ${fileName}`);
        }
    }
})();

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
