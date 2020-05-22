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
            leafNode.name = `叶节点${i * aggs + j}`
            leafNode.value = i * aggs + j
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
                leafNode.name = `叶节点${(i * leaf + j) * leaf + k}`
                leafNode.value = (i * leaf + j) * leaf + k
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
    if(d == 0) {
        tree3level(res)
    }
    else {
        tree4level(res)
    }
}

module.exports = addTree