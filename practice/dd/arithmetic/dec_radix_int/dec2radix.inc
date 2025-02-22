
let radix;
do {
    radix = 2 + Math.trunc(Math.random() * 15);
} while (radix == 10);
const range = radix**digits;

const eRadixHead = document.getElementById("radixHead");
eRadixHead.innerHTML = radix;
const eRadix = document.getElementById("radix");
eRadix.innerHTML = radix;

const number = Math.trunc(Math.random() * range);

const eNumber = document.getElementById("number");
eNumber.innerHTML = number.toString() + "<sub>10</sub>.";

let step = 0;

function makeAnswer() {
    step = 10000;
    makeStep();
}
function firstStep() {
    step = 0;
    makeStep();
}
function makeStep() {
    step++;
    answer();
}

function answer() {

const eHiddens = document.getElementsByClassName("hidden");
for (const e of eHiddens)
    e.style.visibility = "visible";


const eFirtStepButton = document.getElementById("firstStepButton");
eFirtStepButton.style.visibility = "hidden";
const eNextStepButton = document.getElementById("nextStepButton");
eNextStepButton.style.visibility = "visible";

const eAnswerButton = document.getElementById("answerButton");
eAnswerButton.style.visibility = "hidden";

let result = number;
let s = "";
let count = 0;
do {
    let digit = result % radix;
    let result2 = Math.trunc(result / radix);
    s += `<tr>
        <td style="text-align: right;">${result}</td>
        <td style="text-align: center;">${radix}</td>
        <td style="text-align: right;">${result2}</td>
        <td style="text-align: center;">${digit.toString(radix).toUpperCase()}</td></tr>`;
    result = result2;
    count++;
} while (result > 0 && count < step);

const eSolution = document.getElementById("solution");
eSolution.innerHTML = "" + (number % radix ** count).toString(radix).toUpperCase().padStart(count, "0") + `<sub>${radix}</sub>`;

const eCalculation = document.getElementById("calculation");
eCalculation.innerHTML =
     `<table> <tr><th>מחולק</th><th>מחלק</th><th>תוצאה</th><th>שארית</th></tr>
${s} </table>`; 
if (result == 0) 
   eNextStepButton.style.visibility = "hidden";

}
