import {
	getUser
} from '../service/getData'
import {
	GET_USERINFO
} from './mutation-types.js'

export default {
	async getUserInfo({
			commit,
			state
		}) {
			let res = await getUser();
		commit(GET_USERINFO, res)
		},

}