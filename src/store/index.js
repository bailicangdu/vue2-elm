import Vue from 'vue'
import Vuex from 'vuex'
import mutations from './mutations'
import actions from './action'
import getters from './getters'

Vue.use(Vuex)

const state = {
	latitude: '', // 当前位置纬度
	longitude: '', // 当前位置经度
	cartList: {}, // 加入购物车的商品列表
	shopDetail: null, //商家详情信息
	userInfo: null, //用户信息
	shopid: null, // 商铺id
	remarkText: null,//可选择的下单备注
	inputText: '',//手动输入下单备注
	invoice: false,//是否需要开发票，默认否
	searchAddress: null,//添加地址时选择的地址，通过vuex传递给上一级页面
	geohash: null,//在msite页面中存入geohash，这个全局的变量在任何地方都有可能会被用到
	choosedAddress: null,//下单页面确认选择的收货地址
	addressIndex: null,//收获地址的索引值
	needValidation: null,//下单时需要输入验证码的返回值
	cartId: null,//订单的id
	sig: null,//订单的sig
	orderParam: null,//传递给后台的订单参数
	orderMessage: null,//下单成功，保存订单返回信息
	orderDetail: null,//进入订单详情页前保存该订单信息
	login: true,//登陆的状态，默认登陆
}

export default new Vuex.Store({
	state,
	getters,
	actions,
	mutations,

})