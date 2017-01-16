import { RECORD_ADDRESS } from './mutation-types.js'

export default {
	// 记录当前经度纬度
	[RECORD_ADDRESS] (state, {latitude, longitude}) {
		state.latitude = latitude;
		state.longitude = longitude;
	},
}