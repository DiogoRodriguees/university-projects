const meusprojetos = [
    {'id' : '4', 'url': './img/MeusProjetos/calculadora_com_react.jpg'},
    {'id' : '5', 'url': './img/MeusProjetos/img_projetofinancas.png'},
    {'id' : '6', 'url': './img/MeusProjetos/todolista_com_javascript.png'},
]

const conteinerMeusProjetos = document.querySelector('#container')

const  carregarImgs = (meusprojetosImgs, container)=>{
    meusprojetosImgs.forEach(imgItem => {
        container.innerHTML +=
        `<div class='meus-projetos-item'>
            <img src='${imgItem.url}'>
        </div>`
    });
}

carregarImgs( meusprojetos, conteinerMeusProjetos);

let listaDeImgs = document.querySelectorAll('.meus-projetos-item')

const proximoSlide = () =>{
    conteinerMeusProjetos.appendChild( listaDeImgs[0])
    listaDeImgs = document.querySelectorAll('.meus-projetos-item')
}

const slideAnterior = () =>{
    const lastItem = listaDeImgs[listaDeImgs.length -1]
    conteinerMeusProjetos.insertBefore( lastItem, listaDeImgs[0])
    listaDeImgs = document.querySelectorAll('.meus-projetos-item')
}

document.querySelector('#ant').addEventListener('click', slideAnterior)
document.querySelector('#prox').addEventListener('click', proximoSlide)