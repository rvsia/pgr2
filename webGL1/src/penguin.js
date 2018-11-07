class Penguin {
    constructor(scene) {
        var loader = new THREE.OBJLoader();
        var Mtlloader = new THREE.MTLLoader();

        const that = this;
        that.penguin = undefined;

        Mtlloader.load( 'Mesh_Penguin.mtl', function ( mtl ) {
            mtl.preload();
            loader.setMaterials(mtl);
            loader.load( 'Mesh_Penguin.obj', function ( obj ) {
                obj.position.set(0, 0, 0);
                obj.scale.set(1, 1, 1);
                obj.rotation.y = THREE.Math.degToRad(90);

                scene.add( obj );

                that.penguin = obj;

                that.createAnimation();
            }, undefined, function ( error ) {
                console.error( error );
            } );
        }, undefined, function ( error ) {
            console.error( error );
        } );


    }

    rotateRight() {
        this.penguin.rotation.y += 0.1;
    }

    jump() {
        let targetPosition = {z: this.penguin.position.z - 1};
        this.jumpAnimation.to( targetPosition, 2000);
        this.jumpAnimation.start();
    }

    createAnimation() {
        const that = this;
        let penguinPosition = {x: 0, y: 0, z: this.penguin.position.z};
        let targetPosition = {z: this.penguin.position.z - 1};
        var animation = new TWEEN.Tween(penguinPosition);

        animation.to( targetPosition, 2000);
        animation.onUpdate(function () {
            if(that.penguin !== undefined) {
                that.penguin.position.x = penguinPosition.x;
                that.penguin.position.y = penguinPosition.y;
                that.penguin.position.z = penguinPosition.z;
            }})
        animation.easing(TWEEN.Easing.Quadratic.InOut);

        this.jumpAnimation = animation;
    }
}