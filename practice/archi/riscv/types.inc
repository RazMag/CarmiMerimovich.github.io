let rv64 = {};

rv64.registerName = [
     "zero",       // x0
     "ra",
     "sp",
     "gp",
     "tp",
     "t0",         // x5
     "t1",
     "t2",
     "s0",
     "s1",
     "a0",         // x10
     "a1",
     "a2",
     "a3",
     "a4",
     "a5",         // x15
     "a6",
     "a7",
     "s2",
     "s3",
     "s4",         // x20
     "s5",
     "s6",
     "s7",
     "s8",
     "s9",         // X25
     "s10",
     "s11",
     "t3",
     "t4",
     "t5",         // X30
     "t6"
];

rv64.randomInstruction = function () {
     let i = Math.trunc(Math.random() * rv64.types.length);
     let mi = rv64.types[i].machineInstructions;
     
     let ret = {};
     ret.a = {};
     ret.type = mi.type;

     let idx = Math.trunc(Math.random() * mi.info.length);
     let info = mi.info[idx];

     for (let f of ["type", "idFieldName", "operandsFieldName", "miFieldName"]) 
          if (typeof(mi[f]) != "undefined")
               ret[f] = mi[f];

     if (typeof(mi.opCode) != "undefined")
          ret.a.opcode = mi.opCode;


     if (typeof(mi["memory"]) != "undefined")
          ret.a["memory"] = mi["memory"];

     for (let f of ["opcode", "mnem", "fnc3", "fnc5", "fnc6", "fnc7"])
          if (typeof(info[f]) != "undefined")
               ret.a[f] = info[f];

     ret = fillRandomInstruction(ret);

     return (ret);
}


//
// For each instruction class
// (Really for each opcode, as for now)
//
rv64.types = [];

rv64.types.push({        // B
     machineInstructions: {
          opCode: 0b1100011,
          type: "B",
          idFieldName: ["opcode", "fnc3"],   // CPU identifies instruction
          operandsFieldName: ["rs1", "rs2", "imm13"],
          miFieldName: [ // machine instruction format
               "imm13[12]",
               "imm13[10:5]",
               "rs2",
               "rs1",
               "fnc3",
               "imm13[4:1]",
               "imm13[11]",
               "opcode"
          ],
          info: [
               {mnem: "beq",  fnc3: 0b000, desc: "pc &larr; pc + (x[rs1] = x[rs2] ? imm : 4)"},
               {mnem: "bne",  fnc3: 0b001, desc: "pc &larr; pc + (x[rs1] &ne; x[rs2] ? imm : 4)"},
               {mnem: "blt",  fnc3: 0b100, desc: "pc &larr; pc + (x[rs1] <<sub>s</sub> x[rs2] ?  imm : 4)"},
               {mnem: "bge",  fnc3: 0b101, desc: "pc &larr; pc + (x[rs1] &ge;<sub>s</sub> x[rs2] ? imm : 4)"},
               {mnem: "bltu", fnc3: 0b110, desc: "pc &larr; pc + (x[rs1] <<sub>u</sub> x[rs2] ?  imm : 4)"},
               {mnem: "bgeu", fnc3: 0b111, desc: "pc &larr; pc + (x[rs1] &ge;<sub>u</sub> x[rs2] ? imm : 4)"}
          ]
     }
});

rv64.types.push({ // U
     machineInstructions: {
          type: "U",
          idFieldName: ["opcode"],   // CPU identifies instruction
          operandsFieldName: ["rd", "imm20"],
          miFieldName: [ // machine instruction format
               "imm20",
               "rd",
               "opcode"
          ],
          info: [
               {mnem: "lui",   opcode: 0b0110111, desc: "x[rd] &larr; sext(imm << 12)"},
               {mnem: "auipc", opcode: 0b0010111, desc: "x[rd] &larr; pc + sext(imm << 12)"},
          ]
     }
});


rv64.types.push({ // J
     machineInstructions: {
          opCode: 0b1101111,
          type: "J",
          memory: true,
          idFieldName: ["opcode"],   // CPU identifies instruction
          operandsFieldName:  ["rd", "imm21"],
          miFieldName: [ // machine instruction format
               "imm21[20]",
               "imm21[10:1]",
               "imm21[11]",
               "imm21[19:12]",
               "rd",
               "opcode"
          ],
          info: [
               {mnem: "jal", desc: "pc &larr; pc + sext(imm)"}
          ]
     }
});


rv64.types.push({ // S
     machineInstructions: {
          opCode: 0b0100011,
          memory: true,
          type: "S",
          idFieldName: ["opcode", "fnc3"],   // CPU identifies instruction
          operandsFieldName:  ["rs2", "rs1", "imm12"],
          miFieldName: [ // machine instruction format
               "imm12[11:5]",
               "rs2",
               "rs1",
               "fnc3",
               "imm12[4:0]",
               "opcode"
          ],
          info: [
               {mnem: "sb",  fnc3: 0b000, desc: "*(x[rs1] + sext(imm))[7:0] &larr; x[rs2][7:0]"},
               {mnem: "sh",  fnc3: 0b001, desc: "*(x[rs1] + sext(imm))[15:0] &larr; x[rs2][15:0]"},
               {mnem: "sw",  fnc3: 0b010, desc: "*(x[rs1] + sext(imm))[31:0] &larr; x[rs2][31:0]"},
               {mnem: "sd",  fnc3: 0b011, desc: "*(x[rs1] + sext(imm)) &larr; x[rs2]"}
          ]
     }
});

rv64.types.push({ // I  Memory addressing version
     machineInstructions: {
          memory: true,
          type: "I",
          idFieldName:  ["opcode", "fnc3"],   // CPU identifies instruction
          operandsFieldName:  ["rd", "rs1", "imm12"],
          miFieldName: [ // machine instruction format
               "imm12",
               "rs1",
               "fnc3",
               "rd",
               "opcode"
          ],
          info: [
               {mnem: "lb",  opcode: 0b0000011, fnc3: 0b000, desc: "x[rd] &larr; sext(*(x[rs1] + sext(imm))[7:0])"},
               {mnem: "lh",  opcode: 0b0000011,  fnc3: 0b001, desc: "x[rd] &larr; sext(*(x[rs1] + sext(imm))[15:0])"},
               {mnem: "lw",  opcode: 0b0000011,  fnc3: 0b010, desc: "x[rd] &larr; sext(*(x[rs1] + sext(imm))[31:0])"},
               {mnem: "ld",  opcode: 0b0000011,  fnc3: 0b011, desc: "x[rd] &larr; *(x[rs1] + sext(imm))"},
               {mnem: "lbu", opcode: 0b0000011, fnc3: 0b100, desc: "x[rd] &larr; zext(*(x[rs1] + sext(imm))[7:0])"},
               {mnem: "lhu", opcode: 0b0000011, fnc3: 0b101, desc: "x[rd] &larr; zext(*(x[rs1] + sext(imm))[15:0])"},
               {mnem: "lwu", opcode: 0b0000011, fnc3: 0b110, desc: "x[rd] &larr; zext(*(x[rs1] + sext(imm))[31:0])"},
               {mnem: "jalr", opcode: 0b1100111, fnc3: 0b000, desc: "x[rd] &larr; pc + 4; pc &larr; rs1 + sext(imm)"},
          ]
     },
});


rv64.types.push({ // I normal 12-bit constant
     machineInstructions: {
          type: "I",
          idFieldName: ["opcode", "fnc3"],   // CPU identifies instruction
          operandsFieldName:  ["rd", "rs1", "imm12"],
          miFieldName: [ // machine instruction format
               "imm12",
               "rs1",
               "fnc3",
               "rd",
               "opcode"
          ],
          info: [
               {mnem: "addi",  opcode: 0b0010011, fnc3: 0b000, desc: "x[rd] &larr; x[rs1] + sext(imm)"},
               {mnem: "slti",  opcode: 0b0010011, fnc3: 0b010, desc: "x[rd] &larr; x[rs1] <<sub>s</sub> sext(imm) ? 1 : 0"},
               {mnem: "sltiu", opcode: 0b0010011, fnc3: 0b011, desc: "x[rd] &larr; x[rs1] <<sub>u</sub> zext(imm) ? 1 : 0"},
               {mnem: "xori",  opcode: 0b0010011, fnc3: 0b100, desc: "x[rd] &larr; x[rs1] ^ sext(imm)"},
               {mnem: "ori",   opcode: 0b0010011,  fnc3: 0b110, desc: "x[rd] &larr; x[rs1] &or; sext(imm)"},
               {mnem: "andi",  opcode: 0b0010011, fnc3: 0b111, desc: "x[rd] &larr; x[rs1] &and; sext(imm)"},
               {mnem: "addiw", opcode: 0b0011011, fnc3: 0b000, desc: "x[rd] &larr; sext((x[rs1] + sext(imm))[31:0])"},
          ]
     },
});

rv64.types.push({ // I'  6-bit immediate for shifts
     machineInstructions: {
          opCode: 0b0010011,
          type: "I'",
          subType: 1,
          idFieldName: ["opcode", "fnc3", "fnc6"],   // CPU identifies instruction
          operandsFieldName:  ["rd", "rs1", "imm6"],
          miFieldName: [ // machine instruction format
               "fnc6",
               "imm6",
               "rs1",
               "fnc3",
               "rd",
               "opcode"
          ],
          info: [
               {mnem: "slli",  fnc3: 0b001, fnc6: 0b000000, desc: "x[rd] &larr; x[rs1] << imm"},
               {mnem: "srli",  fnc3: 0b101, fnc6: 0b000000, desc: "x[rd] &larr; x[rs1] >><sub>u</sub> imm"},
               {mnem: "srai",  fnc3: 0b101, fnc6: 0b010000, desc: "x[rd] &larr; x[rs1] >><sub>s</sub> imm"},
          ]
     },
});


rv64.types.push({ // I'' 5-bit immediate for shifts
     machineInstructions: {
          opCode: 0b0011011,
          type: "I''",
          subType: 1,
          idFieldName: ["opcode", "fnc3", "fnc7"],   // CPU identifies instruction
          operandsFieldName: ["rd", "rs1", "imm5"],
          miFieldName: [ // machine instruction format
               "fnc7",
               "imm5",
               "rs1",
               "fnc3",
               "rd",
               "opcode"
          ],
          info: [
               {mnem: "slliw",  fnc3: 0b001, fnc7: 0b000000, desc: "x[rd] &larr; sext((x[rs1][31:0] << imm))"},
               {mnem: "srliw",  fnc3: 0b101, fnc7: 0b000000, desc: "x[rd] &larr; sext(x[rs1][31:0] >><sub>u</sub> imm)"},
               {mnem: "sraiw",  fnc3: 0b101, fnc7: 0b010000, desc: "x[rd] &larr; sext(x[rs1][31:0] >><sub>s</sub> imm)"},
          ]
     },
});

rv64.types.push({  // R
     machineInstructions: {
          type: "R",
          idFieldName: ["opcode", "fnc3", "fnc7"],   // CPU identifies instruction
          operandsFieldName:  ["rd", "rs1", "rs2"],
          miFieldName: [ // machine instruction format
               "fnc7",
               "rs2",
               "rs1",
               "fnc3",
               "rd",
               "opcode"
          ],
          info: [
               {mnem: "add",    opcode: 0b0110011, fnc3: 0b000, fnc7: 0b0000000, desc: "x[rd] &larr; x[rs1] + x[rs2]"},
               {mnem: "sub",    opcode: 0b0110011, fnc3: 0b000, fnc7: 0b0100000, desc: "x[rd] &larr; x[rs1] - x[rs2]"},
               {mnem: "sll",    opcode: 0b0110011, fnc3: 0b001, fnc7: 0b0000000, desc: "x[rd] &larr; x[rs1] << x[rs2]"},
               {mnem: "slt",    opcode: 0b0110011, fnc3: 0b010, fnc7: 0b0000000, desc: "x[rd] &larr; x[rs1] <<sub>s</sub> x[rs2] ? 1 : 0"},
               {mnem: "sltu",   opcode: 0b0110011, fnc3: 0b011, fnc7: 0b0000000, desc: "x[rd] &larr; x[rs1] <<sub>u</sub> x[rs2] ? 1 : 0"},
               {mnem: "xor",    opcode: 0b0110011, fnc3: 0b100, fnc7: 0b0000000, desc: "x[rd] &larr; x[rs1] ^ x[rs2]"},
               {mnem: "srl",    opcode: 0b0110011, fnc3: 0b101, fnc7: 0b0000000, desc: "x[rd] &larr; x[rs1] >><sub>u</sub> x[rs2]"},
               {mnem: "sra",    opcode: 0b0110011, fnc3: 0b101, fnc7: 0b0100000, desc: "x[rd] &larr; x[rs1] >><sub>s</sub> x[rs2]"},
               {mnem: "or",     opcode: 0b0110011, fnc3: 0b110, fnc7: 0b0000000, desc: "x[rd] &larr; x[rs1] &or; [rs2]"},
               {mnem: "and",    opcode: 0b0110011, fnc3: 0b111, fnc7: 0b0000000, desc: "x[rd] &larr; x[rs1] 	&and; x[rs2]"},
               {mnem: "mul",    opcode: 0b0110011, fnc3: 0b000, fnc7: 0b0000001, desc: "x[rd] &larr; x[rs1] &times; x[rs2]", ext: "M"},
               {mnem: "mulh",   opcode: 0b0110011, fnc3: 0b001, fnc7: 0b0000001, ext: "M"},
               {mnem: "mulhsu", opcode: 0b0110011, fnc3: 0b010, fnc7: 0b0000001, ext: "M"},
               {mnem: "mulhu",  opcode: 0b0110011, fnc3: 0b011, fnc7: 0b0000001, ext: "M"},
               {mnem: "div",    opcode: 0b0110011, fnc3: 0b100, fnc7: 0b0000001, desc: "x[rd] &larr; x[rs1] /<sub>s</sub> x[rs2]", ext: "M"},
               {mnem: "divu",   opcode: 0b0110011, fnc3: 0b101, fnc7: 0b0000001, desc: "x[rd] &larr; x[rs1] /<sub>u</sub> x[rs2]", ext: "M"},
               {mnem: "rem",    opcode: 0b0110011, fnc3: 0b110, fnc7: 0b0000001, desc: "x[rd] &larr; x[rs1] %<sub>s</sub> x[rs2]", ext: "M"},
               {mnem: "remu",   opcode: 0b0110011, fnc3: 0b111, fnc7: 0b0000001, desc: "x[rd] &larr; x[rs1] %<sub>u</sub> x[rs2]", ext: "M"},
               {mnem: "addw",  opcode: 0b0111011, fnc3: 0b000, fnc7: 0b0000000, desc: "x[rd] &larr; sext((x[rs1] + x[rs2])[31:0])"},
               {mnem: "subw",  opcode: 0b0111011, fnc3: 0b000, fnc7: 0b0100000, desc: "x[rd] &larr; sext((x[rs1] - x[rs2])[31:0])"},
               {mnem: "sllw",  opcode: 0b0111011, fnc3: 0b001, fnc7: 0b0000000, desc: "x[rd] &larr; sext((x[rs1] << x[rs2])[31:0])"},
               {mnem: "srlw",  opcode: 0b0111011, fnc3: 0b101, fnc7: 0b0000000, desc: "x[rd] &larr; sext(x[rs1][31:0] >><sub>u</sub> x[rs2])"},
               {mnem: "sraw",  opcode: 0b0111011, fnc3: 0b101, fnc7: 0b0100000, desc: "x[rd] &larr; sext(x[rs1][31:0] >><sub>s</sub> x[rs2])"},
               {mnem: "mulw",  opcode: 0b0111011, fnc3: 0b000, fnc7: 0b0000001, desc: "x[rd] &larr; sext((x[rs1] &times; x[rs2])[31:0])"},
               {mnem: "divw",  opcode: 0b0111011, fnc3: 0b100, fnc7: 0b0000001, desc: "x[rd] &larr; sext(x[rs1][31:0] /<sub>s</sub> x[rs2][31:0])"},
               {mnem: "divuw", opcode: 0b0111011, fnc3: 0b101, fnc7: 0b0000001, desc: "x[rd] &larr; sext(x[rs1][31:0] /<sub>u</sub> x[rs2][31:0])"},
               {mnem: "remw",  opcode: 0b0111011, fnc3: 0b110, fnc7: 0b0000001, desc: "x[rd] &larr; sext(x[rs1][31:0] %<sub>s</sub> x[rs2][31:0])"},
               {mnem: "remuw", opcode: 0b0111011, fnc3: 0b111, fnc7: 0b0000001, desc: "x[rd] &larr; sext(x[rs1][31:0] %<sub>u</sub> x[rs2][31:0])"},
          ]
     },
});

function fillRandomInstruction(ret) {
     for (let f of ret.operandsFieldName) {
          switch (f) {

          case "imm20":
               ret.a.imm20 = -(2**19) +
                          Math.trunc(Math.random() * (2**20));
               break;

          case "imm21":
               ret.a.imm21 = (-(2**20) +
                          Math.trunc(Math.random() * (2**21))) * 2;
               break;

          case "imm13":
               ret.a.imm13 = (-2048 + Math.trunc(Math.random() * 4096)) * 2;
               break;

          case "imm12":
               ret.a.imm12 = (-2048 + Math.trunc(Math.random() * 4096));
               break;

          case "imm6":
               ret.a.imm6 = Math.trunc(Math.random() * 64);
               break;

          case "imm5":
               ret.a.imm5 = Math.trunc(Math.random() * 32);
               break;

          case "rs1":
          case "rs2":
          case "rd":
               ret.a[f] = Math.trunc(Math.random() * 32);
               break;
          }
     }

     ret["fnc7"] = {
          get bin() {
               return(ret.a.fnc7.toString(2).padStart(7, "0"));
          },

          get num() {
               return (ret.a.fnc7);
          }
     };

     ret["fnc6"] = {
          get bin() {
               return(ret.a.fnc6.toString(2).padStart(6, "0"));
          },

          get num() {
               return (ret.a.fnc6);
          }
     };

     ret["imm21"] = {
          get bin() {
               let n = ret.a.imm21 >= 0 ?
                         ret.a.imm21 :
                         ret.a.imm21 + (2**21);
               let t = n.toString(2).padStart(21, "0");
               return (t);
          },

          get num() {
               return (ret.a.imm21);
          }
     };

     ret["imm20"] = {
          get bin() {
               let n = ret.a.imm20 >= 0 ?
                         ret.a.imm20 :
                         ret.a.imm20 + (2**20);
               let t = n.toString(2).padStart(20, "0");
               return (t);
          },

          get num() {
               return (ret.a.imm20);
          }
     };

     ret["imm13"] = {
          get bin() {
               let n = ret.a.imm13 >= 0 ?
                         ret.a.imm13 :
                         ret.a.imm13 + 8192;
               let t = n.toString(2).padStart(13, "0");
               return (t);
          },

          get num() {
               return (ret.a.imm13);
          }
     };

     ret["imm12"] = {
          get bin() {
               let n = ret.a.imm12 >= 0 ?
                         ret.a.imm12 :
                         ret.a.imm12 + 4096;
               let t = n.toString(2).padStart(12, "0");
               return (t);
          },

          get num() {
               return (ret.a.imm12);
          }
     };

     ret["imm6"] = {
          get bin() {
               let t = ret.a.imm6.toString(2).padStart(6, "0");
               return (t);
          },

          get num() {
               return (ret.a.imm6);
          }
     };

     ret["imm5"] = {
          get bin() {
               let t = ret.a.imm5.toString(2).padStart(5, "0");
               return (t);
          },

          get num() {
               return (ret.a.imm5);
          }
     };

     ret["imm21[20]"] = {
          get bin() {
               let t = ret.imm21.bin.split("").reverse().join("");
               return (t.substring(20, 21));
          },

          get num() {
               let t = this.bin;
               return (parseInt(t, 2));
          }
     };

     ret["imm21[19:12]"] = {
          get bin() {
               let t = ret.imm21.bin.split("").reverse().join("");
               return (t.substring(12, 20).split("").reverse().join(""));
          },

          get num() {
               let t = this.bin;
               return (parseInt(t, 2));
          }
     };


     ret["imm21[11]"] = {
          get bin() {
               let t = ret.imm21.bin.split("").reverse().join("");
               return (t.substring(11, 12));
          },

          get num() {
               let t = this.bin;
               return (parseInt(t, 2));
          }
     };

      ret["imm21[10:1]"] = {
          get bin() {
               let t = ret.imm21.bin.split("").reverse().join("");
               return (t.substring(1, 11).split("").reverse().join(""));
          },

          get num() {
               let t = this.bin;
               return (parseInt(t, 2));
          }
     };
    

     ret["imm13[12]"] = {
          get bin() {
               let t = ret.imm13.bin.split("").reverse().join("");
               return (t.substring(12, 13));
          },

          get num() {
               let t = this.bin;
               return (parseInt(t, 2));
          }
     };

     ret["imm13[11]"] = {
          get bin() {
               let t = ret.imm13.bin.split("").reverse().join("");
               return (t.substring(11, 12));
          },

          get num() {
               let t = this.bin;
               return (parseInt(t, 2));
          }
     };


     ret["imm13[10:5]"] = {
          get bin() {
               let t = ret.imm13.bin.split("").reverse().join("");
               return (t.substring(5, 11).split("").reverse().join(""));
          },

          get num() {
               let t = this.bin;
               return (parseInt(t, 2));
          }
     };

     ret["imm13[4:1]"] = {
          get bin() {
               let t = ret.imm13.bin.split("").reverse().join("");
               return (t.substring(1, 5).split("").reverse().join(""));
          },

          get num() {
               let t = this.bin;
               return (parseInt(t,2));
          }
     };

     ret["imm12[11:5]"] = {
          get bin() {
               let t = ret.imm12.bin.split("").reverse().join("");
               return (t.substring(5, 12).split("").reverse().join(""));
          },

          get num() {
               let t = this.bin;
               return (parseInt(t, 2));
          }
     };

     ret["imm12[4:0]"] = {
          get bin() {
               let t = ret.imm12.bin.split("").reverse().join("");
               return (t.substring(0, 5).split("").reverse().join(""));
          },

          get num() {
               let t = this.bin;
               return (parseInt(t, 2));
          }
     };


     ret["rs1"] = {
          get bin() {
               return (ret.a.rs1.toString(2).padStart(5,"0"));
          },

          get num() {
               return (ret.a.rs1);
          },

          get str() {
               let n = ret.a.rs1;
               let s = Math.random() < 0.5 ?
                         "x"+n.toString() :
                         rv64.registerName[n];
               return (s);
          }
     };

     ret["rs2"] = {
          get bin() {
               return (ret.a.rs2.toString(2).padStart(5,"0"));
          },

          get num() {
               return (ret.a.rs2);
          },

          get str() {
               let n = ret.a.rs2;
               let s = Math.random() < 0.5 ?
                         "x"+n.toString() :
                         rv64.registerName[n];
               return (s);
          }
     };

     ret["rd"] = {
          get bin() {
               return (ret.a.rd.toString(2).padStart(5,"0"));
          },

          get num() {
               return (ret.a.rd);
          },

          get str() {
               let n = ret.a.rd;
               let s = Math.random() < 0.5 ?
                         "x"+n.toString() :
                         rv64.registerName[n];
               return (s);
          }
     };


     ret["fnc3"] = {
          get bin() {
               return (ret.a.fnc3.toString(2).padStart(3,"0"));
          },

          get num() {
               return (ret.a.fnc3);
          }
     };

     ret["opcode"] = {
          get bin() {
               return (ret.a.opcode.toString(2).padStart(7,"0"));
          },

          get num() {
               return (ret.a.opcode);
          }
     };


     ret["mnem"] = {
          get bin() {
               return (ret.a.opcode.toString(2).padStart(7,"0"));
          },

          get num() {
               return (ret.a.opcode);
          }
     };

     ret["mi"] = {
          get assembly() {
               let s = `${ret.a.mnem}\t`;
               let args = [];
               for (let f of ret.operandsFieldName) {
                    if (f == "rd") {
                         args.push(`${ret.rd.str}`);
                         break;
                    }
               }
               for (let f of ret.operandsFieldName) {
                    if (f == "rs1") {
                         args.push(`${ret.rs1.str}`);
                         break;
                    }
               }
               for (let f of ret.operandsFieldName) {
                    if (f == "rs2") {
                         args.push(`${ret.rs2.str}`);
                         break;
                    }
               }
               for (let f of ret.operandsFieldName) {
                    if (f.startsWith("imm")) {
                         args.push(`${ret[f].num}`);
                         break;
                    }
               }
               if (typeof(ret.a.memory) != "undefined"  && ret.a.memory) {
                    let f = args.pop();
                    let rs = args.pop();
                    args.push(`${f}(${rs})`);
               }

               return (s + args.join(","));
          },

          get bin() {
               let mi = "";
               for (let f of ret.miFieldName) {
                    mi += ret[f].bin;
               }
               return (mi);
          },

          get num() {
               return(parseInt(this.bin, 2));
          },

          get hex() {
               return(this.num.toString(16).padStart(8, "0").toUpperCase());
          }
     }

     return(ret);
}

