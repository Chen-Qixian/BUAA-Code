/* const params */
const uploadDest = '/Users/chenqixian/Desktop/tmpFiles'
const port = 8000
/* dependencies */
const express = require('express')
const multer = require('multer') // file handler
const cors = require('cors') // cross origin
const bodyParser = require('body-parser') // express dependency
const fs = require('fs')
/* import js scripts */
const main = require('./resHandler/mainHandler') 
/* initialization */
const app = express()
const upload = multer({ dest: uploadDest }) // file upload path
const singleFile = upload.single('topoFile') // handle single file: paramName
main.initDB()
/* use plug-ins */
app.use(cors())
app.use(bodyParser.urlencoded({ extended: false }))
app.use(bodyParser.json())
/* global vars */
let procNum = '16'
let fileFormat = 'tgt'
/* server actions */
/* read topo file */
app.post('/file', singleFile, (req, res, next) => {
    const file = req.file
    const path = file.path
    // console.log('@@@file', file)
    procNum = file.originalname.match(/\d+/)[0]
    fileFormat = file.originalname.split('.')[1]

    fs.readFile(path, (err, data) => {
        if (err) return console.log(err)
        data = data.toString()
        // console.log(data)
        res.end()
    })
})
/* receive para and send response */
app.get('/para', (req, res) => {
    let data = req.query
    data.procNum = procNum
    data.fileFormat = fileFormat
    // console.log('###data: ',data)
    let response = main.dataRes(data)
    res.json(response)
    res.end()
})

/* server listening port */
app.listen(port, () => {
    console.log(`server start at ${port}`)
})
