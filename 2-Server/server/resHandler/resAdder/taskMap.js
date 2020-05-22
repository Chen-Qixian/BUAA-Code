function addTask(res) {
    const size = 50
    let groupNum = 5
    let nodes = []
    let links = []
    for (let i = 0; i < size; i++) {
        let node = {}
        node.id = i
        node.name = `节点${i}`
        node.itemStyle = null
        node.symbolSize = 16
        node.value = Math.floor(Math.random() * 10) + 10
        node.category = i % 5
        node.draggable = true
        nodes.push(node)
    }
    let cnt = 0
    for (let i = 0; i < size - 1; i++) {
        for (let j = i + 1; j < size; j++) {
            if (i % 5 == j % 5 && i % 2 != 0 && j % 2 != 0) {
                let link = {}
                link.id = cnt++
                link.source = i
                link.target = j
                link.name = null
                link.lineStyle = { normal: {} }
                links.push(link)
            }
            else if (i % 31 == j % 31) {
                let link = {}
                link.id = cnt++
                link.source = i
                link.target = j
                link.name = null
                link.lineStyle = { normal: {} }
                links.push(link)
            }
        }
    }
    links = links.concat([{
        id: cnt++,
        source: 10,
        target: 13,
        name: null,
        lineStyle: { normal: {} }
    },
    {
        id: cnt++,
        source: 33,
        target: 36,
        name: null,
        lineStyle: { normal: {} }
    },
    {
        id: cnt++,
        source: 14,
        target: 17,
        name: null,
        lineStyle: { normal: {} }
    },
    {
        id: cnt++,
        source: 38,
        target: 8,
        name: null,
        lineStyle: { normal: {} }
    },
    {
        id: cnt++,
        source: 18,
        target: 24,
        name: null,
        lineStyle: { normal: {} }
    },
    {
        id: cnt++,
        source: 30,
        target: 24,
        name: null,
        lineStyle: { normal: {} }
    },
    {
        id: cnt++,
        source: 28,
        target: 22,
        name: null,
        lineStyle: { normal: {} }
    },
    {
        id: cnt++,
        source: 22,
        target: 40,
        name: null,
        lineStyle: { normal: {} }
    },
    {
        id: cnt++,
        source: 32,
        target: 22,
        name: null,
        lineStyle: { normal: {} }
    },
    {
        id: cnt++,
        source: 20,
        target: 25,
        name: null,
        lineStyle: { normal: {} }
    },
    {
        id: cnt++,
        source: 26,
        target: 31,
        name: null,
        lineStyle: { normal: {} }
    }
    ])
    res.resultTask = {
        nodes,
        links,
        groupNum
    }
}

module.exports = addTask