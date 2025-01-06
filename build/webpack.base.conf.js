import path from 'path';
import config from '../config';
import utils from './utils';
import { fileURLToPath } from 'url';
import webpack from 'webpack';

const projectRoot = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '../');

const env = process.env.NODE_ENV;
const cssSourceMapDev = env === 'development' && config.dev.cssSourceMap;
const cssSourceMapProd = env === 'production' && config.build.productionSourceMap;
const useCssSourceMap = cssSourceMapDev || cssSourceMapProd;

export default {
  entry: {
    app: './src/main.js',
  },
  output: {
    path: config.build.assetsRoot,
    publicPath: env === 'production' ? config.build.assetsPublicPath : config.dev.assetsPublicPath,
    filename: '[name].js',
  },
  resolve: {
    extensions: ['.js', '.vue', '.less', '.css', '.scss'],
    alias: {
      vue$: 'vue/dist/vue.esm.js',
      src: path.resolve(projectRoot, 'src'),
      assets: path.resolve(projectRoot, 'src/assets'),
      components: path.resolve(projectRoot, 'src/components'),
    },
    modules: [path.resolve(projectRoot, 'node_modules')],
  },
  module: {
    rules: [
      {
        test: /\.vue$/,
        loader: 'vue-loader',
        options: {
          loaders: utils.cssLoaders({ sourceMap: useCssSourceMap }),
          postcss: [
            require('autoprefixer')({
              overrideBrowserslist: ['Android >= 4.4', 'iOS >= 8'],
            }),
          ],
        },
      },
      {
        test: /\.js$/,
        loader: 'babel-loader',
        include: projectRoot,
        exclude: /node_modules/,
      },
      {
        test: /\.json$/,
        loader: 'json-loader',
      },
      {
        test: /\.(png|jpe?g|gif|svg)(\?.*)?$/,
        loader: 'url-loader',
        options: {
          limit: 10000,
          name: utils.assetsPath('img/[name].[ext]'),
        },
      },
      {
        test: /\.(woff2?|eot|ttf|otf)(\?.*)?$/,
        loader: 'url-loader',
        options: {
          limit: 10000,
          name: utils.assetsPath('fonts/[name].[hash:7].[ext]'),
        },
      },
    ],
  },
  plugins: [
    new webpack.DefinePlugin({
      'process.env': {
        NODE_ENV: JSON.stringify(env),
      },
    }),
  ],
};
