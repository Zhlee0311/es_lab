const { SerialPort } = require('serialport');

const port = new SerialPort({
    path: 'COM4',
    baudRate: 9600,
    dataBits: 8,
    stopBits: 1,
    pairty: 'none',
    flowControl: false
});

let flag = false;
let recvData = Buffer.alloc(0);

port.on('open', () => {
    console.log('Port open');
});

port.on('data', (data) => {
    recvData = Buffer.concat([recvData, data]);
    while (recvData.length >= 25) {
        const boardData = recvData.subarray(0, 25);
        console.log('Board Data: ', boardData);
        flag = true;
        console.log('Received 25 bytes of data');
        recvData = recvData.subarray(25);
        coord = nextMove(boardData); // 根据这个函数计算出下一步的坐标
        sendData(coord);
    }
});

function nextMove(board) {
    const SIZE = 5;
    let bestMove = null;

    // 将棋盘1D数组转化为2D数组
    function getBoardArray(board) {
        let temp = Array.from(board);
        let arr = [];
        for (let i = 0; i < SIZE; i++) {
            arr.push(temp.slice(i * SIZE, (i + 1) * SIZE));
        }
        return arr;
    }

    const boardArray = getBoardArray(board);

    // 判断是否可以从 (x1, y1) 移动到 (x2, y2)
    function canMove(fromX, fromY, toX, toY) {
        // 检查目标和源是否在合法范围内
        if (fromX < 0 || fromX >= SIZE || fromY < 0 || fromY >= SIZE ||
            toX < 0 || toX >= SIZE || toY < 0 || toY >= SIZE) {
            return false;
        }

        // 检查源位置是否为黑子（1表示黑子）
        if (boardArray[fromX][fromY] !== 1) {
            return false;
        }

        // 检查目标位置是否为空
        if (boardArray[toX][toY] !== 0) {
            return false;
        }

        // 检查源和目标之间的距离是否小于2
        if (Math.abs(fromX - toX) + Math.abs(fromY - toY) >= 2) {
            return false;
        }

        return true;
    }

    // 判断某一条线上是否有可吃掉的棋子
    function checkForEatingLine(x1, y1, x2, y2) {
        // 水平方向判断
        if (x1 === x2) {
            let line = [];
            for (let i = 0; i < SIZE; i++) {
                line.push(boardArray[x1][i]);
            }
            return line;
        }
        // 垂直方向判断
        if (y1 === y2) {
            let line = [];
            for (let i = 0; i < SIZE; i++) {
                line.push(boardArray[i][y1]);
            }
            return line;
        }
        return [];
    }

    // 判断是否有吃子操作
    function canEat(x1, y1, x2, y2) {
        let line = checkForEatingLine(x1, y1, x2, y2);
        if (line.length === 5) {
            // 规则：连续三个子中有两个是黑子，且有一个是白子
            let blackCount = line.filter(x => x === 1).length;
            let whiteCount = line.filter(x => x === 2).length;
            if (blackCount === 3 && whiteCount === 1) {
                return true; // 可以吃掉对方的子
            }
        }
        return false;
    }

    // 获取所有可移动的合法点
    function findPossibleMoves(boardArray) {
        let possibleMoves = [];
        for (let row = 0; row < SIZE; row++) {
            for (let col = 0; col < SIZE; col++) {
                if (boardArray[row][col] === 1) { // 如果当前位置是黑子
                    // 寻找合法的目标位置
                    for (let newRow = 0; newRow < SIZE; newRow++) {
                        for (let newCol = 0; newCol < SIZE; newCol++) {
                            if (canMove(row, col, newRow, newCol)) {
                                possibleMoves.push([row, col, newRow, newCol]); // 记录合法的移动
                            }
                        }
                    }
                }
            }
        }
        return possibleMoves;
    }

    // 获取所有合法的吃子移动
    function findEatMoves(boardArray) {
        let eatMoves = [];
        for (let row = 0; row < SIZE; row++) {
            for (let col = 0; col < SIZE; col++) {
                if (boardArray[row][col] === 1) { // 如果当前位置是黑子
                    // 寻找可吃的棋子
                    for (let newRow = 0; newRow < SIZE; newRow++) {
                        for (let newCol = 0; newCol < SIZE; newCol++) {
                            if (canEat(row, col, newRow, newCol)) {
                                eatMoves.push([row, col, newRow, newCol]); // 记录吃子的移动
                            }
                        }
                    }
                }
            }
        }
        return eatMoves;
    }

    // 获取所有合法的移动
    const possibleMoves = findPossibleMoves(boardArray);
    const eatMoves = findEatMoves(boardArray);

    // 优先选择吃子操作
    if (eatMoves.length > 0) {
        bestMove = eatMoves[0]; // 选择第一个合法的吃子移动
    } else if (possibleMoves.length > 0) {
        bestMove = possibleMoves[0]; // 选择第一个合法的普通移动
    }

    // 返回最佳移动（格式为 [x1, y1, x2, y2]）
    return bestMove ? bestMove : null;
}



function sendData(data) {
    if (flag) {
        const mergedData = [];
        for (let i = 0; i < data.length; i += 2) {
            const mergedByte = (data[i] << 4) | data[i + 1];
            mergedData.push(mergedByte);
        }
        const dataToSend = Buffer.from(mergedData);
        console.log('Data to send: ', dataToSend);
        port.write(dataToSend, (err) => {
            if (err) {
                console.log('Error writing to port: ', err.message);
            }
            console.log('Data sent to serial port');
        });
        flag = false;
    }
}