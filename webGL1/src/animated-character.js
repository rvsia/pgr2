class AnimatedCharacter {

    constructor(scene) {
        this.scene = scene;

        this.action = {};

        this.loadModels();
    }


    loadModels() {
        const that = this;
        const loader = new THREE.JSONLoader();

        loader.load('./models/eva-animated.json', function (geometry, materials) {
            materials.forEach(function (material) {
                material.skinning = true;
            });

            that.character = new THREE.SkinnedMesh(
                geometry,
                new THREE.MeshFaceMaterial(materials)
            );

            that.mixer = new THREE.AnimationMixer(that.character);

            that.action.hello = that.mixer.clipAction(geometry.animations[ 0 ]);
            that.action.idle = that.mixer.clipAction(geometry.animations[ 1 ]);
            that.action.run = that.mixer.clipAction(geometry.animations[ 3 ]);
            that.action.walk = that.mixer.clipAction(geometry.animations[ 4 ]);

            that.action.hello.setEffectiveWeight(1);
            that.action.idle.setEffectiveWeight(1);
            that.action.run.setEffectiveWeight(1);
            that.action.walk.setEffectiveWeight(1);

            that.action.hello.setLoop(THREE.LoopOnce, 0);
            that.action.hello.clampWhenFinished = true;

            that.action.hello.enabled = true;
            that.action.idle.enabled = true;
            that.action.run.enabled = true;
            that.action.walk.enabled = true;

            that.character.position.set(0, 1, 0);

            that.character.rotation.y = THREE.Math.degToRad(180);

            character.castShadow = true;
            character.receiveShadow = true;

            that.scene.add(that.character);

            that.action.idle.play();

            that.activeActionName = "idle";
            that.isRunning = false;
        });
    }

    update(delta){
        if (this.mixer !== undefined) {
            this.mixer.update(delta);
        }
    }

    setPostion(x,y,z) {
        this.character.position.set(x,y,z);
    }

    setWalk(bool){
        if(bool){
            //this.action.run.play();
            //this.action.idle.stop();
            this.fadeAction("run");
            this.isRunning = true;
        } else {
            //this.action.idle.play();
            //this.action.run.stop();
            this.isRunning = false;
            this.fadeAction("idle");

        }
    }

    fadeAction (name) {
        if(!this.isRunning){
            var from = this.action[ this.activeActionName ].play();
            var to = this.action[ name ].play();

            from.enabled = true;
            to.enabled = true;

            if (to.loop === THREE.LoopOnce) {
                to.reset();
            }

            from.crossFadeTo(to, 0.3);
            this.activeActionName = name;
        }
    }
}