option = {
    title: {
        text: 'CloverLeaf执行时间',
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
            ['64', 27.99, 25.24, 25.01, 25.98, 25.91 ,24.96, 26.01],
            ['128', 26.71, 26.01, 25.32, 25.45, 24.86, 25.89, 25.63],
            ['256', 29.61, 28.82, 27.68, 28.12, 28.32, 27.57],
            ['512', 35.26, 32.47, 32.84, 32.54, 36.10,32.97]
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
