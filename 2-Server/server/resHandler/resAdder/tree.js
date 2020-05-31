function tree3level(res) {
    const core = {}
    const aggs = 4
    const leaf = 4
    core.name = '根节点'
    const aggList = []
    for(let i = 0 ; i < aggs ; i ++) {
        const aggNode = {}
        aggNode.name = `节点${i}`
        const leafList = []
        for(let j = 0 ; j < leaf ; j ++) {
            const leafNode = {}
            leafNode.name = p3[i * aggs + j]
            // leafNode.name = `叶节点${i * aggs + j}`
            leafList.push(leafNode)
        }
        aggNode.children = leafList
        aggList.push(aggNode)
    }
    core.children = aggList
    res.resultTree = core
}

function tree4level(res) {
    const core = {}
    const aggs = 4
    const leaf = 2
    core.name = '根节点'
    const aggList = []
    for(let i = 0 ; i < aggs ; i ++) {
        const aggNode = {}
        aggNode.name = `节点${i}`
        const midList = []
        for(let j = 0 ; j < leaf ; j ++) {
            const leafNode = {}
            leafNode.name = `节点${i * leaf + j}`
            const leafList = []
            for(let k = 0 ; k < leaf ; k ++) {
                const leafNode = {}
                leafNode.name = p4[(i * leaf + j) * leaf + k]
                leafList.push(leafNode)
            }
            leafNode.children = leafList
            midList.push(leafNode)
        }
        aggNode.children = midList
        aggList.push(aggNode)
    }
    core.children = aggList
    res.resultTree = core
}

function addTree(res, d) {
    if(d === 'tgt') {
        tree4level(res)
    }
    else {
        tree3level(res)
    }
}

module.exports = addTree

const p3 = 
[
    '1, 21',
    '7, 28',
    '6, 11',
    '8, 17',
    '5, 18',
    '27, 31',
    '12, 26',
    '22, 25',
    '0, 15',
    '13, 24',
    '2, 23',
    '19, 30',
    '3, 4',
    '9, 10',
    '14, 20',
    '16, 29'
]

const p4 =
[
    '0, 11',
    '5, 28',
    '2, 21',
    '8, 17',
    '7, 18',
    '27, 31',
    '12, 26',
    '16, 25',
    '1, 15',
    '14, 24',
    '6, 23',
    '19, 20',
    '3, 4',
    '9, 10',
    '13, 30',
    '22, 29'
]