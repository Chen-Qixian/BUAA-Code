option = {
    title: {
        text: 'CoMD执行时间',
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
            ['64', 24.86, 23.21, 22.82, 22.80, 22.19, 23.01, 22.45],
            ['128', 19.18, 18.78 , 18.21, 19.09, 20.33 , 18.74, 18.82],
            ['256', 19.96, 17.21 , 17.63, 18.02, 18.52 , 17.30],
            ['512', 21.76, 17.98 , 17.12, 17.92, 17.68 , 18.04]
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
