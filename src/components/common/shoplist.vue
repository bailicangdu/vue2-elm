<template>
	<ul>
		<router-link :to="{path: 'food', query:{}}" v-for="item in shopListArr" tag='li' :key="item.id" class="shop_li">
			<section>
				<img :src="imgBaseUrl + subImgUrl(item.image_path)" class="shop_img">
			</section>
			<hgroup class="shop_right">
				<header class="shop_detail_header">
					<h4 :class="item.is_premium? 'premium': ''" class="" class="shop_title ellipsis">{{item.name}}</h4>
					<ul class="shop_detail_ul">
						<li v-for="item in item.supports" key="item.id" class="supports">{{item.icon_name}}</li>
					</ul>
				</header>
				<h5></h5>
				<h5></h5>
			</hgroup>
		</router-link>
	</ul>
</template>

<script>

import {mapState} from 'vuex'
import {imgBaseUrl} from '../../config/env'
import {msiteShopList} from '../../service/getData'
export default {
	data(){
		return {
			offset: 0, // 批次加载店铺列表，每次加载20个 limit = 20
			shopListArr:[], // 店铺列表数据
			imgBaseUrl,
		}
	},
	async mounted(){
		this.shopListArr = await msiteShopList(this.latitude, this.longitude, this.offset);
	},
	props: [],
	components: {

	},
	computed: {
		...mapState([
			'latitude','longitude'
		])
	},
	methods: {
		subImgUrl(path){
			let suffix;
			if (path.indexOf('jpeg') !== -1) {
				suffix = '.jpeg'
			}else{
				suffix = '.png'
			}
			let url = '/' + path.substr(0,1) + '/' + path.substr(1,2) + '/' + path.substr(3) + suffix;
			return url
		}
	},
	watch: {

	}

}
</script>

<style lang="scss" scoped>
	@import '../../style/mixin.scss';
	.shop_li{
		display: flex;
		border-bottom: 0.025rem solid #f1f1f1;
		padding: 0.8rem 0.4rem;
	}
	.shop_img{
		@include wh(2.7rem, 2.7rem);
		display: block;
		margin-right: 0.3rem;
	}
	.shop_right{
		flex: auto;
		.shop_detail_header{
			@include fj;
			align-items: center;
			.shop_title{
				@include sc(0.7rem, #333);
				font-weight: 700;
				width: 8.5rem;
			}
			.premium{

			}
			.shop_detail_ul{
				display: flex;
				.supports{
					@include sc(0.5rem, #999);
					border: 0.025rem solid #f1f1f1;
					padding: 0.04rem;
					border-radius: 0.08rem;
					margin-left: 0.1rem;
				}
			}
		}
	}

</style>