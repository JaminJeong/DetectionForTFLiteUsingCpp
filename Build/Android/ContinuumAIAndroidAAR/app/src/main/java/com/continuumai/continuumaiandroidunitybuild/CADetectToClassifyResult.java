package com.continuumai.continuumaiandroidunitybuild;


import java.util.ArrayList;

public class CADetectToClassifyResult {

    class Element {
        BoundingBox bbox;
        ClassificationResult clsResult;

        public Element(BoundingBox bbox, ClassificationResult clsResult) {
            this.bbox = bbox;
            this.clsResult = clsResult;
        }
    }

    // Latency (ms).
    public Long latency = -1L;
    // Number of valid detections (separately maintained, maybe different from detections.size()).
    public int count = 0;

    public ArrayList<Element> detections = null;

    public CADetectToClassifyResult() {
        detections = new ArrayList<Element>();
    }

    public void add(BoundingBox box, ClassificationResult clsResult) {
        detections.add(new Element(box, clsResult));
        count += 1;
    }

    public void resetTo(Long latency) {
        count = 0;
        this.latency = latency;
        detections.clear();
    }

    public void OnlyTop1() {
        if (detections.size() > 0) {
            Element one = detections.get(0);
            detections.clear();
            detections.add(one);
        }
    }


    @Override
    public String toString() {
        String textToShow = latency + "ms";
        int k = 0;
        for (Element box : detections) {
            textToShow +=
                    "\nPrediction ["
                            + k
                            + "] = Class : "
                            + box.bbox.category
                            +", "
                            + box.bbox.categoryname
                            + " ("
                            + box.bbox.x1
                            + ", "
                            + box.bbox.y1
                            + ", "
                            + box.bbox.x2
                            + ", "
                            + box.bbox.y2
                            + ") : "
                            + box.bbox.score
                            + "\n";
            if (0 != box.clsResult.info.size()) {
                textToShow += box.clsResult.toString();
            }
            k++;
        }

        return textToShow;
    }

    public String toJson() {
        if (detections.size() == 0) {
            return "";
        }

        String jsonString = "{\"Recog\":[";
        for (Element box : detections) {
            String labelName = null;
            if (box.clsResult.info.size() == 0) {
                labelName = box.bbox.categoryname;
            }
            else {
                labelName = box.clsResult.info.get(0).topKClass;
            }

            jsonString += "{\"ModelName\":\"" + labelName + "\",";
            // if you want Score of Detection
//            jsonString += "\"score\":" +  box.clsResult.info.get(0).topKProb+ ",";
            jsonString += "\"x\":" + box.bbox.x1 + ",";
            jsonString += "\"y\":" + box.bbox.y1 + ",";
            jsonString += "\"w\":" + box.bbox.getWidth() + ",";
            jsonString += "\"h\":" + box.bbox.getHeight() + ",";
            jsonString += "\"score\":" + box.bbox.score;
            jsonString += "},";
        }

        jsonString = jsonString.substring(0, jsonString.length() - 1);
        jsonString += "]}";

        return jsonString;
    }

}
