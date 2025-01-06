import config from '../config';
import path from 'path';
import express from 'express';
import webpack from 'webpack';
import open from 'open';
import { createProxyMiddleware } from 'http-proxy-middleware';
import webpackDevMiddleware from 'webpack-dev-middleware';
import webpackHotMiddleware from 'webpack-hot-middleware';
import history from 'connect-history-api-fallback';
import webpackConfig from './webpack.dev.conf';

if (!process.env.NODE_ENV) process.env.NODE_ENV = JSON.parse(config.dev.env.NODE_ENV);

const port = process.env.PORT || config.dev.port;
const server = express();
const compiler = webpack(webpackConfig);

const devMiddleware = webpackDevMiddleware(compiler, {
  publicPath: webpackConfig.output.publicPath,
  stats: {
    colors: true,
    chunks: false,
  },
});

const hotMiddleware = webpackHotMiddleware(compiler);

compiler.hooks.compilation.tap('HtmlWebpackPluginReload', (compilation) => {
  compilation.hooks.htmlWebpackPluginAfterEmit.tapAsync('HtmlWebpackPluginReload', (data, cb) => {
    hotMiddleware.publish({ action: 'reload' });
    cb();
  });
});

const context = config.dev.context;
let proxypath;
switch (process.env.NODE_ENV) {
  case 'local':
    proxypath = 'http://localhost:8001';
    break;
  case 'online':
    proxypath = 'https://elm.cangdu.org';
    break;
  default:
    proxypath = config.dev.proxypath;
}

if (context.length) {
  server.use(createProxyMiddleware(context, {
    target: proxypath,
    changeOrigin: true,
  }));
}

server.use(history());
server.use(devMiddleware);
server.use(hotMiddleware);

const staticPath = path.posix.join(config.dev.assetsPublicPath, config.dev.assetsSubDirectory);
server.use(staticPath, express.static('./static'));

server.listen(port, (err) => {
  if (err) {
    console.error(err);
    return;
  }
  const uri = `http://localhost:${port}`;
  console.log(`Listening at ${uri}\n`);

  if (process.env.NODE_ENV !== 'testing') {
    open(uri);
  }
});
