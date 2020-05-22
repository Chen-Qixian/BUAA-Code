option = {
    title: {
        text: 'TeaLeaf执行时间',
        left: '50%',
        textAlign: 'center'
    },
    tooltip: {
        trigger: 'axis'
    },
    legend: {
        data: ['自然布局', 'LMPS', 'QTLS', 'TreeMatch', 'APHiD', 'TopoMapping', 'MPIPP'],
        top: 50
    },
    grid: {
        top: 100,
        left: '3%',
        right: '4%',
        bottom: '3%',
        containLabel: true
    },
    toolbox: {
        feature: {
            saveAsImage: {}
        }
    },
    xAxis: {
        type: 'category',
        boundaryGap: false,
        data: ['64', '128', '256', '512']
    },
    yAxis: {
        type: 'value'
    },
    series: [
        {
            name: '自然布局',
            type: 'line',
            data: [10.24, 12.66, 27.91, 30.12]
        },
        {
            name: 'LPMS',
            type: 'line',
            data: [8.31, 10.10, 25.32, 26.14]
        },
        {
            name: 'QTLS',
            type: 'line',
            data: [7.98, 9.11, 25.10, 27.39]
        },
        {
            name: 'TreeMatch',
            type: 'line',
            data: [10.01, 11.24, 28.56, 30.09]
        },
        {
            name: 'APHiD',
            type: 'line',
            data: [7.44, 9.32, 30.23, 36.71]
        },
        {
            name: 'TopoMapping',
            type: 'line',
            data: [8.64, 9.74, 26.65, 27.51]
        },
        {
            name: 'MPIPP',
            type: 'line',
            data: [8.32, 9.63, 0, 0]
        }
    ]
};
