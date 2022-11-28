const imgs = [
    {'id' : '2', 'url': './img/meusprojetos/Captura de Tela (33).png'},
    {'id' : '5', 'url': './img/meusprojetos/Captura de Tela (31).png'},
    {'id' : '3', 'url': './img/meusprojetos/Captura de Tela (31).png'},
    {'id' : '4', 'url': './img/meusprojetos/Captura de Tela (32).png'},
    {'id' : '1', 'url': './img/meusprojetos/Captura de Tela (34).png'},
]
const imgs2 = [
    {'id' : '7', 'url': './img/meusprojetos/sessao_1_light.png'},
    {'id' : '9', 'url': './img/meusprojetos/sessao_2_light.png'},
    {'id' : '8', 'url': './img/meusprojetos/sessao_3_light.png'},
    {'id' : '6', 'url': './img/meusprojetos/sessao_1_light.png'},
]

const conteinerItens = document.querySelector('#conteiner')


const  loadImgs = (imgs, imgs2, conteiner)=>{

        imgs.forEach(img => {
            if(img.id === '5'){
                conteiner.innerHTML +=
                `<div class='item'>
                    <p>Arraste para o lado para conferir as imagens do projeto</p>
                    <img id='img${img.id}' src='${img.url}'>
                </div>`
            }else{
                conteiner.innerHTML +=
                `<div class='item'>
                    <img id='img${img.id}' src='${img.url}'>
                </div>`
            }
        });

        imgs2.forEach(img2 => {
            if(img2.id === '4'){
                conteiner.innerHTML +=
                `<div class='item'>
                    <p>Arraste para o lado para conferir as imagens do projeto</p>
                    <img id='img${img2.id}' src='${img2.url}'>
                </div>`
            }else{
                conteiner.innerHTML +=
                `<div class='item'>
                    <img id='img${img2.id}' src='${img2.url}'>
                </div>`
            }
        });
    
   

}

loadImgs( imgs, imgs2, conteinerItens);

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