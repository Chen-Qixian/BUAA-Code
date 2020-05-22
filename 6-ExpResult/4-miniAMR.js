option = {
    title: {
        text: 'miniAMR执行时间',
        left: '50%',
        textAlign: 'center'
    },
    legend: {
        top: 50
    },
    grid: {
        top: 100,
        left: '3%',
        right: '8%',
        bottom: '5%',
        containLabel: true
    },
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
            ['algorithm', '自然布局', 'LPMS', 'QTLS', 'TreeMatch', 'APHiD', 'TopoMapping', 'MPIPP'],
            ['64', 30.20, 28.78, 28.23, 29.65, 29.73, 28.10, 28.59],
            ['128', 35.12, 31.21 , 33.89, 34.14, 27.88 , 34.67, 34.88],
            ['256', 37.45, 32.02 , 34.23, 34.98, 32.98 , 35.13],
            ['512', 39.22, 35.44 , 36.86, 39.36, 39.54 , 37.48]
        ]
    },
    xAxis: {
        type: 'value',
        name: '并行应用执行时间(s)',
        nameLocation: 'center',
        nameGap: 30,
        max: 40,
        interval: 5
    },
    yAxis: {
        type: 'category',
        name: '并行应用进程数'
    },
    series: [
        {type: 'bar'},
        {type: 'bar'},
        {type: 'bar'},
        {type: 'bar'},
        {type: 'bar'},
        {type: 'bar'},
        {type: 'bar'}
    ]
};
