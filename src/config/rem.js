// https://juejin.im/post/5d477c576fb9a06b2a20236c
// 移动端适配原理
(function(doc, win) {
    var docEl = doc.documentElement,
        resizeEvt = 'orientationchange' in window ? 'orientationchange' : 'resize',
        recalc = function() {
            var clientWidth = docEl.clientWidth;
            if (!clientWidth) return;
            // 320的设计稿 fontSize是20
            docEl.style.fontSize = 20 * (clientWidth / 320) + 'px';
            console.log('fontSize=', docEl.style.fontSize)
        };
    if (!doc.addEventListener) return;
    win.addEventListener(resizeEvt, recalc, false);
    doc.addEventListener('DOMContentLoaded', recalc, false);
})(document, window);