const fs = require('fs')
const path = require('path')
let db = []
const addPreview = require('./resAdder/preview')
const addTree = require('./resAdder/tree')
const addTask = require('./resAdder/taskMap')
const addHeat = require('./resAdder/heat')
const addDownload = require('./resAdder/download')

/** private functions */

/** exported functions */
function initDB() {
    // relative path => absolute path: path.join()
    fs.readFile(path.join(__dirname, './database.json'), (err, data) => {
        if (err) return console.log(err)
        db = JSON.parse(data) // need to transfer Buffer to JSON
    })
}

function dataRes(para) {
    // console.log('###main para: ',para)
    let category = para.method
    let item = 0
    let res = {}
    res = db[category][item] || {}
    addPreview(res, parseInt(category))
    addHeat(res, 0)
    addTask(res)
    addTree(res, 1)
    addDownload(res)
    // console.log(res)
    return res
}

module.exports = {
    initDB,
    dataRes
}