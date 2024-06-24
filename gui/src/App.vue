<template>
  <v-chart class="chart" :option="option" :width="800" height="600" />
  <img id="maskimg" v-bind:src="'R.png'" />
</template>

<script setup>
import { use } from 'echarts/core';
import {
  GraphicComponent, GridComponent, TitleComponent,
  TooltipComponent,
  LegendComponent,
} from 'echarts/components';
import { ScatterChart } from 'echarts/charts';
import { UniversalTransition } from 'echarts/features';
import { CanvasRenderer } from 'echarts/renderers';
import VChart, { THEME_KEY } from 'vue-echarts';
import { ref, provide } from 'vue';

use([
  GraphicComponent,
  GridComponent,
  ScatterChart,
  CanvasRenderer,
  UniversalTransition,
  TitleComponent,
  TooltipComponent,
  LegendComponent,
]);

provide(THEME_KEY, 'dark');

const data = ref(
  [
    [7.5, 5.5, 1, 1],
    [5.8, 8.5, 1, 1],
    [4.5, 9.2, 1, 1],
    [3.4, 8.8, 1, 1],
    [2.5, 7.2, 1, 1]
  ]
);
const rawdata = JSON.parse(JSON.stringify(data.value));
const option = ref({
  title: {
    text: 'Haptic finger Tips result',
    left: 'center',
  },
  // tooltip: {
  //   trigger: 'item',
  //   formatter: '{b0}: {c0}<br />{b1}: {c1}',
  //   // position: 'top'
  // },
  // graphic: {
  //   elements: [
  //     {
  //       type: 'image',
  //       id: 'maskImage',
  //       style: {
  //         image: '/gui/src/assets/R.png', // Replace with the actual URL of the image
  //         x: 0,
  //         y: 0,
  //         width: '100%',
  //         height: '100%'
  //       }
  //     }
  //   ]
  // },
  xAxis: {
    max: 10
  },
  yAxis: {
    max: 10
  },
  series: [
    {
      data: data,
      type: 'scatter',
      symbolSize: function (data) {
        // console.log(data);
        return 20 * data[2]; // Use the third value as the size
      },
      itemStyle: {
        color: function (param) {
          // console.log(param.data[3])
          return "rgb("+Math.floor(255*param.data[3])+", 77, 204)";
        }
      },
      animation: false // Disable transition effects/
    }
  ]
});

setInterval(function () {
  for (let i = 0; i < data.value.length; i++) {
    // data.value[i][0] = rawdata[i][0] + (Math.random() * 0.1 - 0.2);
    // data.value[i][1] = rawdata[i][1] + (Math.random() * 0.1 - 0.2);
    // data.value[i][2] = Math.random();
    data.value[i][3] = Math.random();
  }
}, 100);
</script>

<style scoped>
.chart {
  height: 600px;
  width: 800px;
}

#maskimg {
  position: absolute;
  left: 200px;
  top: 50px;
  opacity: 0.5;
  width: 400px
}
</style>
