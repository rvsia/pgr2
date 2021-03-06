class Lights {
    constructor(scene) {
        this.scene = scene;

        var color = new THREE.Color(0xffffff);
        var light = new THREE.AmbientLight(color, 0.2);
        //light.position.set(5,5,5);
        light.positionX = 5;
        light.positionY = 5;
        light.positionZ = 5;
        scene.add(light);

        this.light = light;
    }

    switchOn() {
        this.light.intensity = 0.1;
    }

    switchOff() {
        this.light.intensity = 0;
    }
}