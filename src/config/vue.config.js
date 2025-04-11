module.exports = {
  devServer: {
    proxy: {
      // 配置代理
      '/api': {
        target: "https://elm.cangdu.org",
        secure: true,
        changeOrigin: true,
        ws: true,
        pathRewrite: { '^/api': '', }
      }
    }
  }
};
