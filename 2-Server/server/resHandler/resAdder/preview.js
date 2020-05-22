function addPreview(res, method) {
    // console.log(res, method)
    const data = [];
    const size = method * 100
    for (let i = 0; i < size; i++) {
        let process = ''
        for (let j = 0; j < 5; j++) {
            process += (i * 6 + j) + ', '
        }
        process += (i * 6 + 5)
        data.push({
            key: i,
            nodeId: `节点 ${i}`,
            coreNum: 32,
            process: process,
        });
    }
    // console.log(data)
    res.resultPreview = data
}


module.exports = addPreview