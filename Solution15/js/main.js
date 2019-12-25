/**
 * 响应回车
 */
function reactEnter() {
    let strval = document.getElementById("inputbox").value;
    if (event.keyCode == 13 && strval != "") {
        search_main();
    }
}


/**
 * 响应按钮点击
 * 传递参数给子页面
 */
function search_main() {
    let strval = document.getElementById("inputbox").value;
    // window.open("s15_sub.html?str="+str);
    window.location.href = "s15_sub.html?str=" + strval;
}