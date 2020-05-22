option = {
    legend: {},
    tooltip: {},
    toolbox: {
        show: true,
        feature: {
            saveAsImage: {
            show:true,
            excludeComponents :['toolbox'],
            pixelRatio: 2
            }
        }
    },
    dataset: {
        source: [
            ['algorithm', 'LPMS', 'QTLS', 'APHiD', 'TreeMatch', 'TopoMapping', 'MPIPP'],
            ['32', 1.2, 1.4, 4.6, 8.4, 148.5, 162.3],
            ['64', 2.3, 2.9, 10.8, 96.4, 294.8, 2192.3],
            ['128', 4.8, 5.2, 30.8 , 149.8, 2921.2, 91901.4],
            ['256', 16.1, 12.4, 60.9, 270.2, 11000.8, 995230.1]
        ]
    },
    xAxis: {
        type: 'category',
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
    // Declare several bar series, each will be mapped
    // to a column of dataset.source by default.
    series: [
        {type: 'bar'},
        {type: 'bar'},
        {type: 'bar'},
        {type: 'bar'},
        {type: 'bar'},
        {type: 'bar'}
    ]
};
