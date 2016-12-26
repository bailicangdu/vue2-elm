import Vue from 'vue'
import Vuex from 'vuex'
import mutations from './mutations'
import actions from './action'
import getters from './getters'
import checkout from './modules/checkout'

Vue.use(Vuex)

const state = {
	test1: null
}

export default new Vuex.Store({
	state,
	getters,
	actions,
	mutations,
	modules: {
		checkout,
	}
})