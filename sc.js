class NeuralNetwork {
    constructor(inputNodes, hiddenLayers, outputNodes) {
        this.inputNodes = inputNodes;
        this.hiddenLayers = hiddenLayers; // Array com o número de nós em cada camada oculta
        this.outputNodes = outputNodes;

        this.weights = [];
        this.biases = [];

        // Criar pesos entre input -> primeira hidden
        this.weights.push(math.random([this.hiddenLayers[0], this.inputNodes], -1, 1));
        this.biases.push(math.random([this.hiddenLayers[0], 1], -1, 1));

        // Criar pesos entre camadas ocultas
        for (let i = 0; i < this.hiddenLayers.length - 1; i++) {
            this.weights.push(math.random([this.hiddenLayers[i + 1], this.hiddenLayers[i]], -1, 1));
            this.biases.push(math.random([this.hiddenLayers[i + 1], 1], -1, 1));
        }

        // Criar pesos entre última hidden -> output
        this.weights.push(math.random([this.outputNodes, this.hiddenLayers[this.hiddenLayers.length - 1]], -1, 1));
        this.biases.push(math.random([this.outputNodes, 1], -1, 1));
    }

    predict(inputsArray) {

        let inputs = math.transpose([inputsArray]);
        let activations = inputs; // Começa com a entrada

        let hiddenOutputs = []; // Para armazenar as ativações das camadas ocultas

        // Passar os dados pelas camadas ocultas
        for (let i = 0; i < this.hiddenLayers.length; i++) {
            activations = math.add(math.multiply(this.weights[i], activations), this.biases[i]);
            activations = math.map(activations, x => 1 / (1 + Math.exp(-x))); // Sigmoid
            hiddenOutputs.push(activations.flat()); // Armazena ativação da camada oculta
        }

        // Passar para a camada de saída
        let output = math.add(math.multiply(this.weights[this.weights.length - 1], activations), this.biases[this.biases.length - 1]);
        output = math.map(output, x => 1 / (1 + Math.exp(-x))); // Sigmoid

        return { hidden: hiddenOutputs, output: output.flat(), inputs: inputs.flat() };
    }

    mutate(rate) {
        this.weights = this.weights.map(w => math.map(w, val => (Math.random() < rate ? val + Math.random() * 0.2 - 0.1 : val)));
        this.biases = this.biases.map(b => math.map(b, val => (Math.random() < rate ? val + Math.random() * 0.2 - 0.1 : val)));
    }

    crossover(partner) {
        let newWeights = [];
        let newBiases = [];
        for (let i = 0; i < this.weights.length; i++) {
            let newMatrix = math.map(this.weights[i], (val, index) => (Math.random() < 0.5 ? val : partner.weights[i].get([index[0], index[1]])));
            newWeights.push(newMatrix);
        }
        for (let i = 0; i < this.biases.length; i++) {
            let newMatrix = math.map(this.biases[i], (val, index) => (Math.random() < 0.5 ? val : partner.biases[i].get([index[0], index[1]])));
            newBiases.push(newMatrix);
        }
        return new NeuralNetwork(this.inputNodes, this.hiddenLayers, this.outputNodes, newWeights, newBiases);
    }
}



class Obstacle {
    static speed = 5
    static Maxspeed = 20

    constructor(isBird = false) {
        this.x = 800;
        this.width = 20;
        this.height = isBird ? 20 : Math.random() * 50 + 20;
        this.isBird = isBird
        // Obstacle.speed = math.random() > .7 ? 10 : 5
    }

    update() {
        this.x -= Obstacle.speed;
    }
}

class Agent {
    constructor(brain = null) {
        this.y = 150;
        this.velocity = 0;
        this.gravity = 0.6;
        this.alive = true;
        this.score = 0;
        this.brain = brain ? brain : new NeuralNetwork(6, [6, 6], 2);
        this.color = `rgb(${Math.floor(Math.random() * 256)}, ${Math.floor(Math.random() * 256)}, ${Math.floor(Math.random() * 256)})`;
    }

    think(obstacles) {
        let inputs = [
            this.y / 150,                   // Normalize by max Dino height (assuming 150 is ground level)
            (obstacles[0]?.x || 800) / 800,  // Normalize by max screen width
            (obstacles[0]?.width || 20) / 100, // Normalize by a reasonable max width (assuming 100px is max)
            (obstacles[0]?.height || 50) / 100, // Normalize by a reasonable max height (assuming 100px max)
            Obstacle.speed / Obstacle.Maxspeed, // Already normalized (0 to 1)
            1 - (Obstacle.speed / Obstacle.Maxspeed), // Alternative representation of speed scaling
            // obstacles[0]?.isBird ? 1 : 0 // No need to normalize, already 0 or 1
        ];
        let outputs = this.brain.predict(inputs);
        if (outputs.output[0] > 0.5) this.jump();
        if (outputs.output[1] > 0.5) this.duck();
        return outputs;
    }

    jump() {
        if (this.y == 150) {
            this.velocity = -10;
        }
    }

    duck() {
        this.y += 10; // Simples abaixar
    }

    update() {
        this.velocity += this.gravity;
        this.y += this.velocity;
        if (this.y > 150) this.y = 150;
        this.score++;
    }

    collides(obstacle) {
        return this.y >= 150 - obstacle.height && obstacle.x < 70 && obstacle.x + obstacle.width > 50;
    }
}
class Game {
    constructor() {
        this.populationSize = 500;
        this.agents = Array.from({ length: this.populationSize }, () => new Agent());
        this.obstacles = [new Obstacle()];
        this.canvas = document.getElementById("gameCanvas");
        this.ctx = this.canvas.getContext("2d");
        this.neuralCanvas = document.getElementById("neuralNetworkCanvas");
        this.neuralCtx = this.neuralCanvas.getContext("2d");
        this.generation = 1;
        this.bestAgent = null;
        this.lastOutput = null;
        setInterval(() => {
            if (Obstacle.speed < Obstacle.Maxspeed)
                Obstacle.speed++
        }, 5000);
    }

    update() {
        let bestDinoLocal = null
        let bestScore = 0
        let bestOutput = null
        this.agents.forEach(agent => {
            if (agent.alive) {
                let outputs = agent.think(this.obstacles);
                agent.update();
                if (this.obstacles.some(obstacle => agent.collides(obstacle))) {
                    agent.alive = false;
                }
                if (agent.score >= bestScore) {

                    bestScore = agent.score
                    bestDinoLocal = agent
                    bestOutput = outputs
                    this.lastOutput = outputs.output;
                    this.bestDino = agent
                }
            }
        });

        if (bestOutput)
            this.drawNeuralNetwork(bestOutput.inputs, bestOutput.hidden);

        if (bestOutput)
            this.drawNeuralNetwork(bestOutput.inputs, bestOutput.hidden);

        this.obstacles.forEach(obstacle => obstacle.update());
        if (this.obstacles[0].x < -this.obstacles[0].width) {
            this.obstacles.shift();
            this.obstacles.push(new Obstacle(Math.random() > .5));
        }
        if (this.agents.every(dino => !dino.alive)) this.nextGeneration();

        this.draw()
    }
    nextGeneration() {

        this.generation++;
        this.agents.sort((a, b) => b.score - a.score);
        let bestDinos = this.agents.slice(0, this.populationSize / 10);
        this.agents = Array.from({ length: this.populationSize }, () => {
            let parent = bestDinos[Math.floor(Math.random() * bestDinos.length)];
            let childBrain = new NeuralNetwork(parent.brain.inputNodes, parent.brain.hiddenNodes, parent.brain.outputNodes);
            childBrain.weightsIH = math.clone(parent.brain.weightsIH);
            childBrain.weightsHO = math.clone(parent.brain.weightsHO);
            childBrain.biasH = math.clone(parent.brain.biasH);
            childBrain.biasO = math.clone(parent.brain.biasO);
            childBrain.mutate(0.1);
            return new Dino(childBrain);
        });
        this.obstacles = [new Obstacle()];
        this.bestDino = null
        Obstacle.speed = 5
    }


    getWeightColor(weight) {
        const normalizedWeight = (weight + 1) / 2; // Normaliza o peso para um valor entre 0 e 1
        const red = Math.floor((1 - normalizedWeight) * 255);
        const green = Math.floor(normalizedWeight * 255);
        return `rgb(${red}, ${green}, 0)`;
    }



    drawNeuralNetwork(inputs, hiddenActivations) {
        const ctx = this.neuralCtx;
        const inputNodes = this.bestDino.brain.inputNodes;
        const outputNodes = this.bestDino.brain.outputNodes;
        const hiddenLayers = this.bestDino.brain.hiddenLayers.length;
        const hiddenNodes = this.bestDino.brain.hiddenLayers;

        ctx.clearRect(0, 0, ctx.canvas.width, ctx.canvas.height);

        const layerX = (layerIndex, totalLayers) => (ctx.canvas.width / (totalLayers + 1)) * (layerIndex + 1);
        const nodeY = (nodeIndex, totalNodes) => (ctx.canvas.height / (totalNodes + 1)) * (nodeIndex + 1);

        let layers = [inputNodes, ...hiddenNodes, outputNodes];
        let positions = [];

        layers.forEach((nodes, layerIndex) => {
            let x = layerX(layerIndex, layers.length);
            let nodePositions = [];

            for (let i = 0; i < nodes; i++) {
                let y = nodeY(i, nodes);
                nodePositions.push({ x, y });

                ctx.beginPath();
                ctx.arc(x, y, 10, 0, Math.PI * 2);

                // Cor dos neurônios

                ctx.fill();
                ctx.stroke();

                // Exibir valor de ativação
                ctx.fillStyle = 'black';

                // Aqui, vamos corrigir o acesso ao índice de inputs para garantir que ele não passe do limite
                let displayValue;
                try {
                    if (layerIndex === 0) {

                        displayValue = inputs[i] !== undefined ? inputs[i].toFixed(2) : "N/A"; // Verifica se inputs[i] existe
                    } else if (layerIndex === layers.length - 1) {
                        displayValue = this.lastOutput?.[i] !== undefined ? this.lastOutput[i].toFixed(2) : "N/A"; // Para a camada de saída
                    } else {
                        displayValue = hiddenActivations[layerIndex - 1]?.[i] !== undefined ? hiddenActivations[layerIndex - 1][i].toFixed(2) : "N/A"; // Camada oculta
                    }

                } catch (error) {
                    debugger
                }
                ctx.fillText(displayValue, x - 10, y - 15);
            }
            positions.push(nodePositions);
        });

        // Desenhar conexões entre todas as camadas
        for (let l = 0; l < positions.length - 1; l++) {
            let weightMatrix = this.bestDino.brain.weights[l];

            for (let i = 0; i < positions[l].length; i++) {
                for (let j = 0; j < positions[l + 1].length; j++) {
                    const weight = weightMatrix?.[j]?.[i] || 0;
                    const color = this.getWeightColor(weight);
                    ctx.strokeStyle = color;
                    ctx.beginPath();
                    ctx.moveTo(positions[l][i].x, positions[l][i].y);
                    ctx.lineTo(positions[l + 1][j].x, positions[l + 1][j].y);
                    ctx.stroke();
                }
            }
        }
    }

    draw() {
        const { ctx } = this
        ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);
        ctx.fillStyle = "black";

        this.agents
            .filter(agent => agent.alive)
            .forEach(agent => {
                ctx.fillRect(50, agent.y, 20, 20)
            })
        this.obstacles.forEach(obstacle => {

            ctx.fillRect(obstacle.x, 170 - obstacle.height, obstacle.width, obstacle.height)
        });
        ctx.fillText(`Generation: ${this.generation}`, 10, 10);
        ctx.fillText(`Speed: ${Obstacle.speed}`, 10, 20);
        ctx.fillText(`Alive: ${this.agents.filter(d => d.alive).length}`, 10, 30);
        ctx.fillText(`Bird: ${this.obstacles[0].isBird}`, 10, 40);

        ctx.beginPath()
        const h = 170
        ctx.moveTo(0, h)
        ctx.lineTo(ctx.canvas.width, h)
        ctx.stroke()
    }
}





let game = new Game();
let velocity = 1
setInterval(() => {
    for (let i = 0; i < velocity; i++) {
        game.update()

    }
}, 1000 / 60);


