 <template>
    <div class="search_address_page">
        <section>
            <head-top head-title="搜索" go-back='true'></head-top>
            <form class="search_form">
                <input type="search" name="search" placeholder="请输入小区/写字楼/学校等" v-model="searchValue">
                <button @click.prevent="searchPlace()">搜索</button>
            </form>
            <ul class="address_list" v-if="searchData">
                <li v-for="(item, index) in searchData" :key="index" @click="choooedAddress(item)">
                    <h4>{{item.name}}</h4>
                    <p>{{item.address}}</p>
                </li>
            </ul>
            <div v-else class="empty_tips">
                <p>找不到地址？</p>
                <p>尝试输入小区、写字楼或学校名</p>
                <p>详细地址（如门牌号等）可稍后输入哦</p>
            </div>
        </section>
    </div>
</template>

<script>
    import headTop from '../../../../../components/header/head'
    import {searchNearby} from '../../../../../service/getData'
    import {mapMutations} from 'vuex'

    export default {
      data(){
            return{
               	searchValue: null,//搜索的信息
                searchData: null,//搜索返回的地址列表
            }
        },
        components: {
            headTop,
        },
        props:[],
        methods: {
            ...mapMutations([
                'CHOOSE_SEARCH_ADDRESS'
            ]),
            //搜索附近地址
            async searchPlace(){
                if (this.searchValue) {
                    this.searchData = await searchNearby(this.searchValue);
                }
            },
            choooedAddress(item){
                //将选择的地址传入vuex
                this.CHOOSE_SEARCH_ADDRESS(item);
                //返回上一页，添加地址页
                this.$router.go(-1);
            },

        }
    }
</script>
  
<style lang="scss" scoped>
    @import '../../../../../style/mixin';
  
    .search_address_page{
        position: fixed;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        background-color: #fff;
        z-index: 204;
        overflow-y: auto;
        padding-top: 1.95rem;
        p, span{
            font-family: Helvetica Neue,Tahoma,Arial;
        }
    }
    .search_form{
        display: flex;
        padding: .7rem;
        input{
            @include sc(.65rem, #999);
            flex: 4;
            background-color: #f1f1f1;
            margin-right: .6rem;
            height: 1.5rem;
            border-radius: 0.15rem;
            padding: 0 .4rem;
        }
        button{
            flex: 1;
            @include sc(.65rem, #fff);
            background-color: $blue;
            border-radius: 0.15rem;
        }
    }
    .address_list{
        padding: .7rem;
        li{
            padding: .7rem 0;
            border-bottom: 0.025rem solid #f5f5f5;
            line-height: 1rem;
            h4{
                @include sc(.75rem, #555);
            }
            p{
                @include sc(.65rem, #999);
            }
        }
    }
    .empty_tips{
        @include center;
        width: 100%;
        p{
            @include sc(.5rem, #aaa);
            line-height: .7rem;
            text-align: center;

        }
    }
</style>
