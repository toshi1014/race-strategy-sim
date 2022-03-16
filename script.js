const STRAIGHT = 500;
const RADIUS = 150;
const MARGIN = {
    x: 100,
    y: 100,
};
const DOT_RADIUS = 5;

let resultFile = null;

const COLOR_LIST = [
    "red",
    "blue",
    "green",
    "yellow",
    "orange",
];

// split circuit into 4 sector
//               A
//       +-------|-------+
//   D   |               |   B
//       |               |
//       +---------------+
//               C

const SECTORS = {
    A: STRAIGHT / 2,
    B: STRAIGHT / 2 + (RADIUS + DOT_RADIUS) * 2 * Math.PI / 2,
    C: STRAIGHT / 2 + (RADIUS + DOT_RADIUS) * 2 * Math.PI / 2 + STRAIGHT,
    D: STRAIGHT / 2 + (RADIUS + DOT_RADIUS) * 2 * Math.PI + STRAIGHT,
};


window.onload = function() {
    const canvas = document.getElementById("canvas");
    const ctx = canvas.getContext("2d");
    drawCircuit(ctx, STRAIGHT, RADIUS, MARGIN);
}

const drawCircuit = (ctx) => {
    // oval
    ctx.beginPath();
    ctx.arc(
        MARGIN.x + RADIUS,
        RADIUS + MARGIN.y,
        RADIUS,
        -Math.PI / 2,
        Math.PI / 2,
        true
    );
    ctx.arc(
        MARGIN.x + RADIUS + STRAIGHT,
        RADIUS + MARGIN.y,
        RADIUS,
        Math.PI / 2,
        -Math.PI / 2,
        true
    );
    ctx.closePath();
    ctx.stroke();

    // control line
    ctx.moveTo(
        MARGIN.x + RADIUS + STRAIGHT / 2,
        0
    );
    ctx.lineTo(
        MARGIN.x + RADIUS + STRAIGHT / 2,
        MARGIN.y
    );
    ctx.stroke();
}

const loadSettings = async (input) => {
    const file = input.files[0];
    resultFile = JSON.parse(await file.text());
    console.log("result_file:", resultFile);
}

const getXy = (percent) => {
    const perimeter = STRAIGHT * 2 + (RADIUS + DOT_RADIUS) * 2 * Math.PI;
    const length = perimeter * percent;
    let x, y;

    if (length < SECTORS.A) {
        x = MARGIN.x + RADIUS + STRAIGHT / 2 + length;
        y = MARGIN.y - DOT_RADIUS * 2;

    } else if (length < SECTORS.B) {
        let diff = length - SECTORS.A;
        const o = {
            x: MARGIN.x + RADIUS + STRAIGHT,
            y: MARGIN.y + RADIUS,
        };
        const center_angle = diff / (RADIUS + DOT_RADIUS);
        x = o.x + Math.sin(center_angle) * (RADIUS + DOT_RADIUS);
        y = o.y - Math.cos(center_angle) * (RADIUS + DOT_RADIUS);

    } else if (length < SECTORS.C) {
        let diff = length - SECTORS.B;
        x = MARGIN.x + RADIUS + STRAIGHT - diff;
        y = MARGIN.y + RADIUS * 2 + DOT_RADIUS * 2;

    } else if (length < SECTORS.D) {
        let diff = length - SECTORS.C;

        const o = {
            x: MARGIN.x + RADIUS,
            y: MARGIN.y + RADIUS,
        };
        const center_angle = diff / (RADIUS + DOT_RADIUS);
        x = o.x - Math.sin(center_angle) * (RADIUS + DOT_RADIUS);
        y = o.y + Math.cos(center_angle) * (RADIUS + DOT_RADIUS);

    } else { // sector A again
        let diff = length - SECTORS.D;

        x = MARGIN.x + RADIUS + diff;
        y = MARGIN.y - DOT_RADIUS * 2;
    }

    return {
        x: x,
        y: y
    };
}

const step = (state) => {
    if (state === undefined) {
        clearInterval(timeId);
        console.log("finish");
    } else {
        const canvas = document.getElementById("canvas");
        const ctx = canvas.getContext("2d");

        ctx.clearRect(0, 0, canvas.width, canvas.height);

        drawCircuit(ctx);

        for (let car of state) {
            const xy = getXy(car["distance"]);

            ctx.beginPath();
            ctx.arc(
                xy.x,
                xy.y,
                DOT_RADIUS * 2,
                0,
                Math.PI * 2,
                true
            );
            ctx.fillStyle = COLOR_LIST[car["car_num"] - 1];
            ctx.fill();
            ctx.closePath();
        }
        console.log("state:", state);
    }
}

const start = () => {
    if (resultFile) {
        let idx = 0;
        timeId = setInterval(() => {
            step(resultFile["lap_chart"][idx]), idx++
        }, 500);
    } else {
        alert("Load settins.json beforehand")
    }
}