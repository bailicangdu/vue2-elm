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
	shopid: null,//商铺id
	remarkText: null,//可选备注内容
	inputText: '',//输入备注内容
	invoice: false,//开发票
	searchAddress: null,//搜索并选择的地址
	geohash: null,//地址geohash值
	choosedAddress: null,//选择地址
	addressIndex: null,//选择地址的索引值
	needValidation: null,//确认订单时是否需要验证
	cartId: null, //购物车id
	sig: null,//购物车sig
	orderParam: null,//订单的参数
	orderMessage: null, //订单返回的信息
	orderDetail: null, //订单详情
	login: true,//是否登陆
	imgPath:null,//头像地址
	removeAddress:[],//移除地址
	question: null,//问题详情
}

export default new Vuex.Store({
	state,
	getters,
	actions,
	mutations,
})