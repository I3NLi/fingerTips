
import { createApp } from 'vue'
import App from './App.vue'
import router from './router'

import './assets/main.css'

const app = createApp(App)

app.use(router)

app.mount('#app')
// import * as echarts from 'echarts';

// var chartDom = document.getElementById('main');
// var myChart = echarts.init(chartDom);
// var option;

// let data = [
//   [2, 6, 1],
//   [3.7, 8.5, 1],
//   [5, 9, 1],
//   [6, 8.5, 1],
//   [7.0, 7, 1]
// ];

// option = {
//   graphic: {
//     elements: [
//       {
//         type: 'image',
//         id: 'maskImage',
//         style: {
//           image: '', // Replace with the actual URL of the image
//           x: 0,
//           y: 0,
//           width: '100%',
//           height: '100%'
//         }
//       }
//     ]
//   },
//   xAxis: {
//     max: 10
//   },
//   yAxis: {
//     max: 10
//   },
//   series: [
//     {
//       data: data,
//       type: 'scatter',
//       symbolSize: function (data) {
//         console.log(data);
//         return 20 * data[2]; // Use the third value as the size
//       },
//       animation: false // Disable transition effects/
//     }
//   ]
// };

// setInterval(function () {
//   for (let i = 0; i < 5; i++) {
//     data[i][2] = Math.random();
//   }
// //   console.log(data);
//   myChart.setOption(option);
// }, 1000);

// option && myChart.setOption(option);