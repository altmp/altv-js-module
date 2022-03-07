// This tool is used to convert the results of a JS module profiler .samples
// file to a .json file.

import * as fs from "fs";

// [0]     [1]           [2]           [3]
// node profiler.js path/to/input path/to/output
const fileName = process.argv[2];
let outName = process.argv[3];
if (!outName)
    outName = fileName.replace(".samples", "") + ".json";

if (!fileName.endsWith(".samples") || !fs.existsSync(fileName)) {
    console.error("Invalid path specified");
    process.exit(1);
}

const file = fs.readFileSync(fileName, "utf8");

const results = [];

const lines = file.split("\n");
for (const line of lines) {
    if (line.length === 0)
        continue;
    const [name, samplesStr] = line.split(" ");

    const samples =
        samplesStr.split(",").map(x => parseInt(x)).sort((a, b) => a - b);

    const sampleAmount = samples.length;
    const min = samples[0];
    const max = samples[sampleAmount - 1];

    const avg = samples.reduce((a, b) => a + b, 0) / sampleAmount;

    results.push({
        name,
        samples: sampleAmount,
        min,
        max,
        avg
    });
    console.log(`${name} | Samples: ${sampleAmount} | Min: ${min} | Max: ${max} | Avg: ${avg}`);
}

fs.writeFileSync(outName, JSON.stringify(results, null, 4));
console.log(`Wrote result to '${outName}'`);
