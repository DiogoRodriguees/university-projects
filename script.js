const imgs = [
    {'id' : '3', 'url': './img/slideshow/calculadora_com_react.jpg'},
    {'id' : '2', 'url': './img/slideshow/img_projetofinancas.png'},
    {'id' : '1', 'url': './img/slideshow/todolista_com_javascript.png'},
]

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

let listaDeItens = document.querySelectorAll('.item')

const proximoSlide = () =>{
    conteinerItens.appendChild( listaDeItens[0])
    listaDeItens = document.querySelectorAll('.item')
}

const slideAnterior = () =>{
    const lastItem = listaDeItens[listaDeItens.length -1]
    conteinerItens.insertBefore( lastItem, listaDeItens[0])
    listaDeItens = document.querySelectorAll('.item')
}

document.querySelector('#previous').addEventListener('click', slideAnterior)
document.querySelector('#next').addEventListener('click', proximoSlide)