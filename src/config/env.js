/**
 * 配置编译环境和线上环境之间的切换
 * 
 * baseUrl : 路由地址域名
 * 
 */
let baseUrl;  
if (process.env.NODE_ENV == 'development') {
	baseUrl = '';
}else{
	baseUrl = 'https://mainsite-restapi.ele.me';
}

export {baseUrl}