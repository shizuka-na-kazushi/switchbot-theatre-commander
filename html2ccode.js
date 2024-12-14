const fs = require("node:fs");
const readline = require("node:readline");

const ico = fs.readFileSync("./speaker.ico");

const f = readline.createInterface(fs.createReadStream("sample.html"));

let str = "";

f.on("line", (lineString) => {
  const l = lineString.replaceAll("\"", "\\\"");
  str += ("client.println(\"" + l + "\");\n");
})

f.on("close", () => {
  const fwrite = fs.openSync("./output.cpp", fs.constants.O_CREAT | fs.constants.O_RDWR | fs.constants.O_TRUNC);
  fs.writeFileSync(fwrite, str);  
})

