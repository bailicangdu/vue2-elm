 <template>
	<div class="rating_page">
       <head-top head-title="评论" go-back='true'></head-top>
    </div>
</template>

<script>
	import headTop from '../../../components/header/head'

    export default {
    	data(){
            return{
               
            }
        },
        mounted(){
        	
        },
        components: {
        	headTop,
        },
        props:[],
    }
</script>
	
<style lang="scss" scoped>
    @import '../../../style/mixin.scss';
	
	.rating_page{
		position: absolute;
		top: 0;
		left: 0;
		right: 0;
		bottom: 0;
		background-color: #fff;
		z-index: 18;
	}
</style>
