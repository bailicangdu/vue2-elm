var config = require('../config')
if (!process.env.NODE_ENV) process.env.NODE_ENV = JSON.parse(config.dev.env.NODE_ENV)
var path = require('path')
var express = require('express')
var webpack = require('webpack')
var opn = require('opn')
var proxyMiddleware = require('http-proxy-middleware')
var webpackConfig = require('./webpack.dev.conf')

// default port where dev server listens for incoming traffic
var port = process.env.PORT || config.dev.port
    // Define HTTP proxies to your custom API backend
    // https://github.com/chimurai/http-proxy-middleware

var server = express()
var compiler = webpack(webpackConfig)

var devMiddleware = require('webpack-dev-middleware')(compiler, {
    publicPath: webpackConfig.output.publicPath,
    stats: {
        colors: true,
        chunks: false
    }
})

var hotMiddleware = require('webpack-hot-middleware')(compiler)
    // force page reload when html-webpack-plugin template changes
compiler.plugin('compilation', function(compilation) {
    compilation.plugin('html-webpack-plugin-after-emit', function(data, cb) {
        hotMiddleware.publish({
            action: 'reload'
        })
        cb()
    })
})

var context = config.dev.context
var proxypath = config.dev.proxypath

var options = {
    target: proxypath,
    changeOrigin: true,
}
if (context.length) {
    server.use(proxyMiddleware(context, options))
}

server.use(proxyMiddleware('/payapi', {
    target: 'https://pay.ele.me',
    changeOrigin: true,
}))
server.use(proxyMiddleware('/m.ele.me@json', {
    target: 'https://crayfish.elemecdn.com',
    changeOrigin: true,
}))



// handle fallback for HTML5 history API
server.use(require('connect-history-api-fallback')())

// serve webpack bundle output
server.use(devMiddleware)

// enable hot-reload and state-preserving
// compilation error display
server.use(hotMiddleware)

// serve pure static assets
var staticPath = path.posix.join(config.dev.assetsPublicPath, config.dev.assetsSubDirectory)
server.use(staticPath, express.static('./static'))

module.exports = server.listen(port, function(err) {
    if (err) {
        console.log(err)
        return
    }
    var uri = 'http://localhost:' + port
    console.log('Listening at ' + uri + '\n')

    // when env is testing, don't need open it
    if (process.env.NODE_ENV !== 'testing') {
        opn(uri)
    }
})