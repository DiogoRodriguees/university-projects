
// const imgs = [
//     {'id' : '1', 'url': './img/IMG_CALCULATOR_IPHONE.png'},
//     {'id' : '4', 'url': './img/colorido.jpg'},
// ]

const conteinerItens = document.querySelector('#conteiner')

const  loadImgs = (imgs, conteiner)=>{
    imgs.forEach(img => {
        conteiner.innerHTML +=
        `<div class='item'>
        <img src='${img.url}'>
        </div>`
    });
}

loadImgs( imgs, conteinerItens);

let itens = document.querySelectorAll('.item')

const previous = () =>{
    conteinerItens.appendChild( itens[0])
    itens = document.querySelectorAll('.item')
}

const next = () =>{
    const lastItem = itens[itens.length -1]
    conteinerItens.insertBefore( lastItem, itens[0])
    itens = document.querySelectorAll('.item')
}

document.querySelector('#previous').addEventListener('click', previous)
document.querySelector('#next').addEventListener('click', next)