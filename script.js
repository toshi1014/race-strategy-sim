const STRAIGHT = 500;
const RADIUS = 150;
const MARGIN = {
    x: 50,
    y: 50,
};
const DOT_RADIUS = 5;
const INTERVAL = 100;

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

const loadResultFile = async (input) => {
    const file = input.files[0];
    resultFile = JSON.parse(await file.text());
    updateLeaderboard(resultFile["lap_chart"][0]);
    step(resultFile["lap_chart"][0]);
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
        const centerAngle = diff / (RADIUS + DOT_RADIUS);
        x = o.x + Math.sin(centerAngle) * (RADIUS + DOT_RADIUS);
        y = o.y - Math.cos(centerAngle) * (RADIUS + DOT_RADIUS);

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
        const centerAngle = diff / (RADIUS + DOT_RADIUS);
        x = o.x - Math.sin(centerAngle) * (RADIUS + DOT_RADIUS);
        y = o.y + Math.cos(centerAngle) * (RADIUS + DOT_RADIUS);

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

const updateLeaderboard = (state) => {
    const leaderboard = document.getElementById("leaderboard");

    // clear last one
    if (leaderboard.firstElementChild)
        leaderboard.removeChild(leaderboard.firstElementChild);

    let rowList = [];
    let table = document.createElement("table");

    const lapNow = state.find(carTmp => carTmp.position === 1)["lap"];
    table.appendChild(document.createTextNode(`LAP ${lapNow} / ${resultFile["num_of_laps"]}`));

    rowList.push(table.insertRow(-1));

    // header
    for (let item of ["Pos.", "No.", "Lap", "Tire", "Age", "Remarks"]) {
        let cell = rowList[0].insertCell(-1);
        cell.appendChild(document.createTextNode(item));
        cell.style.backgroundColor = "#bbb";
    }

    // content
    for (let pos = 1; pos <= state.length; pos++) {
        const car = state.find(carTmp => carTmp.position === pos);
        rowList.push(table.insertRow(-1));

        let remarks = "";
        if (car["in_pit"]) remarks = "IN PIT";
        if (car["checkered"]) remarks = "FIN";

        for (let item of [
                pos,
                car["car_num"],
                car["lap"],
                car["compound"],
                car["age"],
                remarks,
            ]) {
            let cell = rowList[pos].insertCell(-1);
            cell.appendChild(document.createTextNode(item));

            cell.style.backgroundColor = "#ddd";
        }
    }

    document.getElementById("leaderboard").appendChild(table);
}

const step = (state) => {
    if (state === undefined) {
        clearInterval(timeId);
        alert("FINISH");
    } else {
        const canvas = document.getElementById("canvas");
        const ctx = canvas.getContext("2d");

        ctx.clearRect(0, 0, canvas.width, canvas.height);
        drawCircuit(ctx);

        // draw cars
        for (let car of state) {
            const xy = getXy(car["distance"]);
            if (car["in_pit"]) xy.y += DOT_RADIUS * 2 * 2;
            else if (car["checkered"]) xy.y -= DOT_RADIUS * 2 * 2;

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

            ctx.fillText(car["car_num"], xy.x, xy.y - DOT_RADIUS * 2)

            if (car["in_pit"])
                ctx.fillText("IN PIT", xy.x, xy.y - DOT_RADIUS * 2 * 2)
        }

        updateLeaderboard(state);
    }
}

const start = () => {
    if (resultFile) {
        let idx = 0;
        timeId = setInterval(() => {
            step(resultFile["lap_chart"][idx]), idx++
        }, INTERVAL);
    } else {
        alert("Load settins.json beforehand")
    }
}