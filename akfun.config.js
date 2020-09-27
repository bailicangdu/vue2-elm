'use strict';
const path = require('path');
// 统一路径解析
function resolve(dir) {
  return path.resolve(__dirname, dir);
}

// 包括生产和开发的环境配置信息
module.exports = {
  settings: {
    enableEslint: true, // 调试模式是否开启ESLint，默认开启ESLint检测代码格式
    enableEslintFix: false, // 是否自动修正代码格式，默认不自动修正
    enableStyleLint: true, // 是否开启StyleLint，默认开启ESLint检测代码格式
    enableStyleLintFix: false // 是否需要StyleLint自动修正代码格式
  },
  webpack: {
    entry: {
      // webpack构建入口
      index: './src/main.js',
    },
    resolve: {
      // webpack的resolve配置
      extensions: ['.js', '.jsx', '.vue', 'json'], // 用于配置webpack在尝试过程中用到的后缀列表
      alias: {
        '@': resolve('src'),
        'vue$': 'vue/dist/vue.common.js',
        'src': resolve('src'),
        'assets': resolve('src/assets'),
        'components':resolve('src/components'),
      },
    },
    template: resolve('./index.html'), // 默认html模板
    // sassResources中的sass文件会自动注入每一个sass文件中
    sassResources: [
      resolve('./src/style/common.scss'),
      resolve('./src/style/mixin.scss'),
    ],
  },
  // envParams：项目系统环境变量(根据执行命令中的环境变量批量替换项目源码中的相关参数)
  envParams: {
    common: {
      // 通用参数
      '#version#': '20200810.1',
    },
    local: {
      // 本地开发环境
      '#dataApiBase#': 'http://localhost:1024', // 数据接口根地址
      '#assetsPublicPath#': 'http://localhost:1024', // 静态资源根地址
      '#routeBasePath#': '/', // 路由根地址
    },
    online: {
      // 线上正式环境配置参数
      '#dataApiBase#': '/', // 数据接口根地址 "//goodtool666.cn/"格式
      '#assetsPublicPath#': '', // 静态资源根地址 "//goodtool666.cn/_spa/sportNews"格式
      '#routeBasePath#': '/', // 路由根地址 "/_spa/sportNews/"格式
    },
  },
  dev: {
    // 用于开启本地调试模式的相关配置信息
    NODE_ENV: 'development',
    port: 80,
    autoOpenBrowser: true,
    assetsPublicPath: '/', // 设置静态资源的引用路径（根域名+路径）
    assetsSubDirectory: '',
    hostname: 'localhost',
    cssSourceMap: false,
    proxyTable: {
      '/shopping': {
        target: 'http://cangdu.org:8001',
        ws: true,
        changeOrigin: true
      },
      '/ugc': {
        target: 'http://cangdu.org:8001',
        ws: true,
        changeOrigin: true
      },
      '/v1': {
        target: 'http://cangdu.org:8001',
        ws: true,
        changeOrigin: true
      },
      '/v2': {
        target: 'http://cangdu.org:8001',
        ws: true,
        changeOrigin: true
      },
      '/v3': {
        target: 'http://cangdu.org:8001',
        ws: true,
        changeOrigin: true
      },
      '/v4': {
        target: 'http://cangdu.org:8001',
        ws: true,
        changeOrigin: true
      },
      '/bos': {
        target: 'http://cangdu.org:8001',
        ws: true,
        changeOrigin: true
      },
      '/member': {
        target: 'http://cangdu.org:8001',
        ws: true,
        changeOrigin: true
      },
      '/promotion': {
        target: 'http://cangdu.org:8001',
        ws: true,
        changeOrigin: true
      },
      '/eus': {
        target: 'http://cangdu.org:8001',
        ws: true,
        changeOrigin: true
      },
      '/payapi': {
        target: 'http://cangdu.org:8001',
        ws: true,
        changeOrigin: true
      },
      '/img': {
        target: 'http://cangdu.org:8001',
        ws: true,
        changeOrigin: true
      }
    },
  },
  build: {
    // 用于构建生产环境代码的相关配置信息
    NODE_ENV: 'production',
    assetsRoot: resolve('./dist'), // 打包后的文件绝对路径（物理路径）
    assetsPublicPath: '/', // 设置静态资源的引用路径（根域名+路径）
    assetsSubDirectory: '', // 资源引用二级路径
    productionSourceMap: false,
    productionGzip: false,
    productionGzipExtensions: ['js', 'css', 'json'],
    bundleAnalyzerReport: false,
  },
};
