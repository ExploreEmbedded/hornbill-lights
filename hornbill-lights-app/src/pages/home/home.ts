import { Component, Input} from '@angular/core';
import { NavController, AlertController } from 'ionic-angular';
import { ConnectPage } from '../connect/connect';
import { BLE } from 'ionic-native';
import { ModalController } from 'ionic-angular';
import { HornbillBlueService } from '../../app/hornbill-blue.service';
import { ColorPalletePage } from '../color-pallete/color-pallete';




@Component({
  selector: 'page-home',
  templateUrl: 'home.html'
})


export class HomePage {

  public connectStatus:boolean = false;  //check if the phone is connected to the bluetooth device
  public HORNBILL_DEVICE_ID:string = "";
  public HORNBILL_OUR_SERVICE:string = "00ff";
  public HORNBILL_OUR_CHARACTERISTICS:string= "ff01";


  public effects: { id: number, name: string, colorPallet:boolean, palletShow:boolean, colorRed:number, colorGreen:number, colorBlue:number, repeat:boolean, play:boolean}[] = [
      { "id": 0, "name": "Solid", "colorPallet":true, "palletShow":false, "colorRed":65, "colorGreen":210, "colorBlue":210, "repeat":false, "play":false},
      { "id": 1, "name": "Rainbow", "colorPallet":false, "palletShow":false, "colorRed":65, "colorGreen":210, "colorBlue":0, "repeat":false, "play":false},
      { "id": 2, "name": "Chase", "colorPallet":false, "palletShow":false, "colorRed":0, "colorGreen":99, "colorBlue":99, "repeat":false, "play":false},
      { "id": 3, "name": "Fade In Fade Out", "colorPallet":true, "palletShow":false, "colorRed":35, "colorGreen":120, "colorBlue":54, "repeat":false, "play":false},
      { "id": 4, "name": "Automagic", "colorPallet":false, "palletShow":false, "colorRed":108, "colorGreen":210, "colorBlue":56, "repeat":false, "play":false},
  ];

  public repeatEffectID:number;


  constructor(private hornbillBlueService:HornbillBlueService, public navCtrl: NavController, public alertCtrl: AlertController,private modalCtrl: ModalController, ) {
    //this.checkBluetooth();
    //console.log(this.hornbillBlueService.getInfo());

  }

  //check if bluetooth is enabled on the device.
  checkBluetooth(){
  }

  //ask user to turn on bluetooth
  showConfirm() {
      let confirm = this.alertCtrl.create({
      title: 'Turn ON Bluetooth',
      message: 'Looks like phone bluetooth is disabled, enable it and retry!',
      buttons: [
      {
        text: 'Okay',
        handler: () => {
          //console.log('Okay');
        }
      }]
      });
      confirm.present();
  }

  // start scanning this should be moved to connect page
  startScanning(){
    //console.log("Scanning Started");
    BLE.scan([],2).subscribe(device => {
    });
  }

 //handler for connect switch on home page.
  public connectOur(){

    const modal = this.modalCtrl.create(ConnectPage);
    if(this.connectStatus){
      //console.log("disconnecting.....");
      this.connectStatus = false;
      // ToDo :disconnect ble later
    }
    else{
      //console.log("connecting.....");
      BLE.isEnabled().then(
        ()=>{
            //console.log("Bluetooth is enabled on device");
            modal.present();
            modal.onDidDismiss((connectStatusReturned:boolean, deviceId:string) => {
                //console.log("deviceid returned "+ deviceId);
                this.HORNBILL_DEVICE_ID = deviceId;
                this.connectStatus = connectStatusReturned;
            });
        },
        ()=>{
        //    console.log("show message to user");
            this.showConfirm();
        }
      );
    }
    //this.connectStatus =!this.connectStatus ;
  }

  //effect functions
  repeatEffect(effectID:number){

  }

  playEffect(effectId:number){
    console.log(this.effects[effectId]);
    var newPack = new Uint8Array(4);
    newPack[0] = this.effects[effectId].colorRed; //first byte
    newPack[1] = this.effects[effectId].colorGreen; //second byte
    newPack[2] = this.effects[effectId].colorBlue; // third byte
    newPack[3] = effectId; //ToDo: can be moved to  a services page and assigned dynamically.
    //console.log(newPack.buffer);
    BLE.write(this.HORNBILL_DEVICE_ID, this.HORNBILL_OUR_SERVICE, this.HORNBILL_OUR_CHARACTERISTICS, newPack.buffer);
  }

}
