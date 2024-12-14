

const fs = require("node:fs");


const binfiles = 
[
  {path: "./speaker.ico",  varname: "favicon_data" },
  {path: "./switch-on.png",  varname: "switch_on_data" },
  {path: "./switch-off.png",  varname: "switch_off_data" },
]

let arrayString = "";

binfiles.forEach((e) => {
  const ico = fs.readFileSync(e.path);

  // console.log(ico);
  const length = ico.length;
  
  arrayString += `const uint8_t ${e.varname}[] PROGMEM = {`;
  let i = 0;
  ico.forEach((v) => {
    arrayString += ("0x" + v.toString(16));
    if (i < length) {
      arrayString += (", ");
    }
    i++;
  });
  
  arrayString += "};\n\n";  
})

console.log(arrayString);

const fwrite = fs.openSync("./output.h", fs.constants.O_CREAT | fs.constants.O_RDWR);
fs.writeFileSync(fwrite, arrayString);
