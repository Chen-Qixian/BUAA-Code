option = {
    title: {
        text: '算法执行时间开销对比',
        left: '50%',
        textAlign: 'center'
    },
    tooltip: {
        trigger: 'axis'
    },
    legend: {
        data: ['LMPS', 'QTMS', 'TreeMatch', 'APHiD', 'TopoMapping', 'MPIPP'],
        top: 50
    },
    grid: {
        top: 100,
        left: '3%',
        right: '8%',
        bottom: '5%',
        containLabel: true
    },
    toolbox: {
        feature: {
            saveAsImage: {
                show: true,
                type: 'png',
                pixelRatio:5
            }
        }
    },
    xAxis: {
        type: 'category',
        boundaryGap: false,
        data: ['32', '64', '128', '256'],
        name: '并行应用进程数',
        nameLocation: 'center',
        nameGap: 30
    },
    yAxis: {
        type: 'log',
        logbase: 10,
        min: 1,
        max: 1000000,
        name: '算法执行时间（ms）'
    },
    series: [
        {
            name: 'LPMS',
            type: 'line',
            data: [1.2, 2.3, 4.8, 16.1]
        },
        {
            name: 'QTMS',
            type: 'line',
            data: [1.4, 2.9, 5.2, 12.4]
        },
        {
            name: 'TreeMatch',
            type: 'line',
            data: [8.4, 96.4, 149.8, 270.2]
        },
        {
            name: 'APHiD',
            type: 'line',
            data: [4.6, 10.8, 30.8, 60.9]
        },
        {
            name: 'TopoMapping',
            type: 'line',
            data: [148.5, 294.8, 2921.2, 11000.8]
        },
        {
            name: 'MPIPP',
            type: 'line',
            data: [162.3, 2192.2, 91901.4, 995230.1]
        }
    ]
};
