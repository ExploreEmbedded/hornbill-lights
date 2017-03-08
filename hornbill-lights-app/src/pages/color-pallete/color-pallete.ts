import { Component } from '@angular/core';
import { NavController, NavParams } from 'ionic-angular';
import { ViewController } from "ionic-angular";


/*
  Generated class for the ColorPallete page.

  See http://ionicframework.com/docs/v2/components/#navigation for more info on
  Ionic pages and navigation.
*/
@Component({
  selector: 'page-color-pallete',
  templateUrl: 'color-pallete.html'
})
export class ColorPalletePage {

  private redValue:number = 0;
  private greenValue:number= 0x90;
  private blueValue:number = 0x90;

  constructor(public navCtrl: NavController, public navParams: NavParams, private viewCtrl: ViewController) {}

  ionViewDidLoad() {
    console.log('ionViewDidLoad ColorPalletePage');
  }

  updateColor(){
    console.log("\n\r update \t"+ this.redValue +"\t"+ this.greenValue +"\t"+ this.blueValue);

    //this.viewCtrl.dismiss(this.redValue, this.blueValue, this.greenValue );
    this.viewCtrl.dismiss({redValue:this.redValue, blueValue:this.blueValue, greenValue:this.greenValue});

    //newPack[0] = this.redValue;
    //newPack[1] = this.greenValue;
    //newPack[2] = this.blueValue;
    //newPack[3] = 0 ; //use this for effects.

    //BLE.write(this.HORNBILL_DEVICE_ID, this.HORNBILL_OUR_SERVICE, this.HORNBILL_OUR_CHARACTERISTICS, newPack.buffer);

  }

}
