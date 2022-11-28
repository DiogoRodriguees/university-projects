const imgs = [
    {'id' : '2', 'url': './img/meusprojetos/Captura de Tela (33).png'},
    {'id' : '5', 'url': './img/meusprojetos/Captura de Tela (31).png'},
    {'id' : '3', 'url': './img/meusprojetos/Captura de Tela (31).png'},
    {'id' : '4', 'url': './img/meusprojetos/Captura de Tela (32).png'},
    {'id' : '1', 'url': './img/meusprojetos/Captura de Tela (34).png'},
]
const imgs2 = [
    {'id' : '2', 'url': './img/meusprojetos/sessao_1_light.png'},
    {'id' : '5', 'url': './img/meusprojetos/sessao_2_light.png'},
    {'id' : '3', 'url': './img/meusprojetos/sessao_3_light.png'},
    {'id' : '1', 'url': './img/meusprojetos/sessao_1_light.png'},
]

const conteinerItens = document.querySelector('#conteiner')


const  loadImgs = (imgs,  conteiner)=>{

        imgs.forEach(img => {

            if(img.id === '5'){
                conteiner.innerHTML +=
                `<div class='item'>
                    <p>Arraste para o lado para conferir as imagens do projeto</p>
                    <img src='${img.url}'>
                </div>`
            }else{
                conteiner.innerHTML +=
                `<div class='item'>
                <img src='${img.url}'>
                </div>`
            }
        });
    
   

}

loadImgs( imgs, conteinerItens);

let listaDeItens = document.querySelectorAll('.item')

const nextSlide = () =>{
    conteinerItens.appendChild( listaDeItens[0])
    listaDeItens = document.querySelectorAll('.item')
}

const previousSlide = () =>{
    const lastItem = listaDeItens[listaDeItens.length -1]
    conteinerItens.insertBefore( lastItem, listaDeItens[0])
    listaDeItens = document.querySelectorAll('.item')
}

document.querySelector('#previous').addEventListener('click', previousSlide)
document.querySelector('#next').addEventListener('click', nextSlide)