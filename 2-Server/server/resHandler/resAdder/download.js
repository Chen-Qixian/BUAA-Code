function addDownload (res, method, procNum) {
    const algo = ['', 'LPMS', 'QTMS', 'TopoMapping', 'APHiD', 'TreeMatch', 'MPIPP']
    if(method === 0) {
        res.downloadPath = 'output'
    }
    else {
        res.downloadPath = `${procNum}/${algo[method]}`
    }   
}

module.exports = addDownload