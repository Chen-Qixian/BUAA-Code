option = {
    title: {
        text: 'TeaLeaf执行时间',
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
            ['64', 10.24, 8.31, 7.98, 10.01, 7.44, 8.64, 8.32],
            ['128', 12.66, 10.10, 9.11, 11.24, 9.32, 9.74, 9.63],
            ['256', 27.91, 25.32, 25.10, 28.56, 30.23, 26.65],
            ['512', 30.12, 26.14, 27.39, 30.09, 31.71, 27.51]
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
