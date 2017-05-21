/**
 * 配置编译环境和线上环境之间的切换
 * 
 * localapi: 是否使用本地API接口
 * proapi: 是否使用线上API接口
 * baseUrl: 域名地址
 * routerMode: 路由模式
 * imgBaseUrl: 图片所在域名地址
 * 
 */
let localapi = false;
let proapi = false;
let baseUrl = ''; 
let routerMode = 'hash';
let imgBaseUrl = 'https://fuss10.elemecdn.com';
let ApiUrl = '';

if (localapi) {
	ApiUrl = 'http://localhost:8001';
	imgBaseUrl = 'http://localhost:8001';
}else if(proapi){
	ApiUrl = 'http://cangdu.org:8001';
	imgBaseUrl = 'http://cangdu.org:8001';
}
if (process.env.NODE_ENV == 'development') {
	
}else if(process.env.NODE_ENV == 'production'){
	proapi = true;
	ApiUrl = 'http://localhost:8001';
	imgBaseUrl = 'http://localhost:8001';
}

export {
	baseUrl,
	routerMode,
	imgBaseUrl,
	ApiUrl,
	localapi,
	proapi,
}