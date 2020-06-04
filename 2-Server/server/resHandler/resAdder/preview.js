function addPreview(res, method) {
    // console.log('@@@',method)
    const data = []
    const size = s[method]
    let proc = []
    let tmp = p[method].split(' ')
    let tmpStr = ''
    for(let i = 0 ; i < tmp.length ; i ++) {
        tmpStr += tmp[i]
        if((i + 1) % c[method] === 0) {
            proc.push(tmpStr)
            tmpStr = ''
        }
        else {
            tmpStr += ', '
        }
    }

    for (let i = 0; i < size; i++) {
        data.push({
            key: i,
            nodeId: `节点 ${i}`,
            coreNum: c[method],
            process: proc[i],
        });
    }
    // console.log(data)
    res.resultPreview = data
}

module.exports = addPreview

const c = [
    0,
    4,
    4,
    4,
    8,
    4,
    4
]
const p = [
    '',
    '0 3 1 6 14 4 8 11 12 9 5 13 7 10 15 2',
    '3 5 28 20 19 18 23 2 27 30 31 17 8 15 10 24 22 9 6 14 25 29 1 12 4 13 16 21 0 7 26 11',
    '77 103 5 8 28 82 93 79 20 32 23 122 22 124 31 113 76 26 125 64 45 110 54 50 0 86 36 3 4 30 100 35 43 90 126 10 58 33 29 39 34 83 59 56 127 42 53 117 112 111 27 62 37 95 60 52 97 107 25 99 24 65 85 101 68 120 72 108 92 48 80 55 21 15 44 75 67 13 2 91 119 61 69 94 89 7 14 71 121 96 16 109 116 66 11 9 49 46 12 40 41 38 81 105 78 19 114 17 47 118 123 115 87 18 1 84 57 73 63 98 74 102 70 106 88 104 51 6',
    '61 26 29 8 55 42 13 34 59 51 5 47 53 25 40 19 60 18 35 15 49 52 37 62 32 57 10 48 28 9 39 63 16 33 1 31 50 54 27 41 21 11 6 0 44 12 2 22 20 56 17 36 14 38 3 7 4 23 58 46 30 43 45 24',
    '12 5 0 8 11 7 6 14 9 4 10 1 2 15 3 13',
    '0 7 6 8 15 2 5 14 13 11 10 12 1 9 3 4'
]
const s = [
    0,
    4,
    8,
    32,
    8,
    4,
    4
]


