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
	shopid: null,
	remarkText: null,
	inputText: '',
	invoice: false,
	searchAddress: null,
	geohash: null,
	choosedAddress: null,
	addressIndex: null,
	needValidation: null,
	cartId: null,
	sig: null,
	orderParam: null,
	orderMessage: null,
}

export default new Vuex.Store({
	state,
	getters,
	actions,
	mutations,

})