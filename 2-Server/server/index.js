/* const params */
const uploadDest = '/Users/chenqixian/Desktop/MyBUAA/0-Codes/2-Server/server/upload_files'
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
/* server actions */
app.post('/file', singleFile, (req, res, next) => {
    const file = req.file
    const path = file.path

    fs.readFile(path, (err, data) => {
        if (err) return console.log(err)
        data = data.toString()
        // console.log(data)
        res.end("1")
    })
})

app.get('/para', (req, res) => {
    let data = req.query
    // console.log('###data: ',data)
    let response = main.dataRes(data)
    res.json(response)
    res.end()
})

/* server listening port */
app.listen(port, () => {
    console.log(`server start at ${port}`)
})
