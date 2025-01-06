import path from 'path';
import webpack from 'webpack';
import { merge } from 'webpack-merge';
import HtmlWebpackPlugin from 'html-webpack-plugin';
import config from '../config';
import utils from './utils';
import baseWebpackConfig from './webpack.base.conf';

// Add hot-reload related code to entry chunks
Object.keys(baseWebpackConfig.entry).forEach((name) => {
  baseWebpackConfig.entry[name] = ['./build/dev-client', ...baseWebpackConfig.entry[name]];
});

export default merge(baseWebpackConfig, {
  module: {
    rules: utils.styleLoaders({
      sourceMap: config.dev.cssSourceMap,
    }),
  },
  // eval-source-map is faster for development
  devtool: 'eval-source-map',
  plugins: [
    new webpack.DefinePlugin({
      'process.env': config.dev.env,
    }),
    // Hot Module Replacement plugins
    new webpack.HotModuleReplacementPlugin(),
    new webpack.NoEmitOnErrorsPlugin(),
    // Generate index.html with correct assets hash for caching
    new HtmlWebpackPlugin({
      filename: 'index.html',
      template: 'index.html',
      favicon: 'favicon.ico',
      inject: true,
    }),
  ],
});
