 <template>
    <div class="rating_page">
        <head-top head-title="选择地址" go-back='true'></head-top>
        <router-link to="/confirmOrder/chooseAddress/addAddress" class="add_icon_footer" >
            <img src="../../../images/add_address.png" height="24" width="24">
            <span>新增收货地址</span>
        </router-link>
        <ul class="deliverable_address">
            <li v-for="(item,index) in deliverable" @click="chooseAddress(item, index)">
                <svg class="choosed_address" :class="{default_address: defaultIndex == index}">
                    <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#select"></use>
                </svg>
                <div>
                    <header>
                        <span>{{item.name}}</span>
                        <span>{{item.sex == 1? '先生' : '女士'}}</span>
                        <span>{{item.phone}}</span>   
                    </header>
                    <div class="address_detail ellipsis">
                        <span v-if="item.tag" :style="{backgroundColor: iconColor(item.tag)}">{{item.tag}}</span>
                        <p>{{item.address_detail}}</p>   
                    </div>
                </div>
            </li>
        </ul>
        <section id="out_delivery" v-if="deliverdisable.length">
            <header class="out_header">以下地址超出配送范围</header>
            <ul class="deliverable_address">
                <li v-for="(item,index) in deliverdisable">
                    <svg class="choosed_address">
                        <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#select"></use>
                    </svg>
                    <div>
                        <header>
                            <span>{{item.name}}</span>
                            <span>{{item.sex == 1? '先生' : '女士'}}</span>
                            <span>{{item.phone}}</span>   
                        </header>
                        <div class="address_detail ellipsis">
                            <span v-if="item.tag" :style="{backgroundColor: '#ccc'}">{{item.tag}}</span>
                            <p>{{item.address_detail}}</p>   
                        </div>
                    </div>
                </li>
            </ul>
        </section>
        <alert-tip v-if="showAlert" @closeTip="showAlert = false" :alertText="alertText"></alert-tip>
        <transition name="router-slid">
            <router-view></router-view>
        </transition>  
    </div>
</template>

<script>
    import headTop from '../../../components/header/head'
    import {mapState, mapMutations} from 'vuex'
    import {getAddress} from '../../../service/getData'
    import alertTip from '../../../components/common/alertTip'

    export default {
      data(){
            return{
               	addressList: [],//地址列表
                deliverable: [],//地址列表中符合配送范围的地址
                deliverdisable: [],//地址列表中，超出配送范围的值
                id: null,//订单id
                sig: null,//订单sig
                showAlert: false,//控制弹出提示框
                alertText: null,//弹出框的文字
            }
        },
        created(){
            this.id = this.$route.query.id;
            this.sig = this.$route.query.sig;
            this.initData();
        },
        components: {
            headTop,
            alertTip,
        },
        props:[],
        computed: {
            ...mapState([
                'userInfo', 'addressIndex'
            ]),
            //默认选择的地址
            defaultIndex: function (){
                if (this.addressIndex) {
                    return this.addressIndex;
                }else{
                    return 0;
                }
            }
        },
        methods: {
            ...mapMutations([
                'CHOOSE_ADDRESS'
            ]),
            //初始化获取数据
            async initData(){
                //用户未登陆时弹出提示框
                if (!(this.userInfo && this.userInfo.user_id)) {
                    this.showAlert = true;
                    this.alertText = '请登陆'
                    return
                }
                //获取地址列表
                this.addressList = await getAddress(this.id, this.sig);
                //将地址列表中在配送范围和不在配送范围的地址分开
                this.addressList.forEach(item => {
                    //满足配送范围
                    if (item.is_deliverable) {
                        this.deliverable.push(item);
                    //不在配送范围
                    }else{
                        this.deliverdisable.push(item);
                    }
                    
                })
            },
            //地址标识的背景颜色
            iconColor(name){
                switch(name){
                    case '公司': return '#4cd964';
                    case '学校': return '#3190e8';
                }
            },
            //选择地址时，将地址的信息，索引传入vuex，然后返回订单页面
            chooseAddress(address, index){
                this.CHOOSE_ADDRESS({address, index});
                this.$router.go(-1);
            },
        }
    }
</script>
  
<style lang="scss" scoped>
    @import '../../../style/mixin';
  
    .rating_page{
        position: fixed;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        background-color: #f5f5f5;
        z-index: 204;
        padding-top: 1.95rem;
        p, span{
            font-family: Helvetica Neue,Tahoma,Arial;
        }
    }
    .add_icon_footer{
        position: fixed;
        bottom: 0;
        left: 0;
        right: 0;
        height: 2.5rem;
        background-color: #fff;
        display: flex;
        justify-content: center;
        align-items: center;
        z-index: 204;
        span{
            @include sc(.7rem, $blue);
            margin-left: .3rem;
        }
    }
    .deliverable_address{
        background-color: #fff;
        li{
            display: flex;
            align-items: center;
            border-bottom: 0.025rem solid #f5f5f5;
            padding: .7rem;
            line-height: 1rem;
            .choosed_address{
                @include wh(.8rem, .8rem);
                fill: #4cd964;
                margin-right: .4rem;
                opacity: 0;
            }
            .default_address{
                opacity: 1;
            }
            header{
                @include sc(.7rem, #333);
                span:nth-of-type(1){
                    font-size: .8rem;
                    font-weight: bold;
                }
            }
            .address_detail{
                width: 100%;
                display: flex;
                align-items: center;
                span{
                    @include sc(.5rem, #fff);
                    border-radius: .15rem;
                    background-color: #ff5722;
                    height: .6rem;
                    line-height: .6rem;
                    padding: 0 .2rem;
                    margin-right: .3rem;
                }
                p{
                    @include sc(.6rem, #777);
                }
            }
        }
    }
    #out_delivery{
        .out_header{
            @include sc(.6rem, #666);
            line-height: 1.5rem;
            padding-left: .5rem;
        }
        *{
            color: #ccc;
        }
    }
    .router-slid-enter-active, .router-slid-leave-active {
        transition: all .4s;
    }
    .router-slid-enter, .router-slid-leave-active {
        transform: translateX(100%);
    }
</style>
